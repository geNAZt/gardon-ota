package firmware

import (
	"crypto/md5"
	"encoding/hex"
	"gardon.local/server/client"
	"gardon.local/server/network/packet"
	"github.com/fsnotify/fsnotify"
	"log"
	"math"
	"os"
)

const CHUNK_SIZE uint16 = 32767

type Firmware struct {
	checksum string

	chunks [][]byte
	size   uint32

	cb func(string)
}

func NewFirmware(cb func(string)) *Firmware {
	fw := &Firmware{
		cb: cb,
	}

	fw.updateChecksum()
	go fw.check()
	return fw
}

func (f *Firmware) check() {
	watcher, err := fsnotify.NewWatcher()
	if err == nil {
		err = watcher.Add("firmware/firmware.bin")
		if err == nil {
			for {
				select {
				case m := <-watcher.Events:
					log.Printf("Got new fs event: %s\n", m.Op)
					f.updateChecksum()
				}
			}
		}
	}
}

func (f *Firmware) updateChecksum() {
	sum := md5.New()
	file, err := os.Open("firmware/firmware.bin")
	if err != nil {
		log.Printf("Could not check for new firmware: %v\n", err)
		return
	}

	stat, err := file.Stat()
	if err != nil {
		log.Printf("Could not check for new firmware: %v\n", err)
		return
	}

	// Calc amount of chunks we need
	log.Printf("Firmware size: %d\n", stat.Size())
	chunkAmount := math.Ceil(float64(stat.Size()) / float64(CHUNK_SIZE))
	log.Printf("Got %f chunks of firmware\n", chunkAmount)

	// Read in file in chunks so we can send them more easily
	chunks := make([][]byte, int(chunkAmount))
	for i := 0; i < int(chunkAmount); i++ {
		buf := make([]byte, CHUNK_SIZE)
		n, err := file.Read(buf)
		if err != nil {
			log.Printf("Could not copy file into hasher: %v\n", err)
			return
		}

		sum.Write(buf[:n])
		chunks[i] = buf[:n]
	}

	// Get the 16 bytes hash
	hashInBytes := sum.Sum(nil)[:16]

	newSum := hex.EncodeToString(hashInBytes)
	if newSum != f.checksum {
		// Convert the bytes to a string
		f.checksum = newSum
		f.chunks = chunks
		f.size = uint32(stat.Size())
		f.cb(f.checksum)
	}
}

func (f *Firmware) Checksum() string {
	return f.checksum
}

func (f *Firmware) SendUpdate(client client.Client) {
	// Send OTA start first
	client.Write(packet.NewOTAPacketStart(packet.FIRMWARE, f.size, f.checksum))

	// Now send all chunks
	for _, chunk := range f.chunks {
		client.Write(packet.NewOTAPacketWithChunk(packet.CHUNK, chunk))
	}

	// Now send the end so it flashes and restarts
	client.Write(packet.NewOTAPacket(packet.END, packet.FIRMWARE))

}
