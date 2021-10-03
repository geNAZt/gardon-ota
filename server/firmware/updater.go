package firmware

import (
	"bytes"
	"crypto/md5"
	"crypto/sha256"
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

	// Get last 32 bytes and make a sha256 from all others
	seek, err := file.Seek(-32, 2)
	if err != nil {
		log.Printf("Could not seek to end in new firmware: %v\n", err)
		return
	}

	log.Printf("Seeked to %d\n", seek)
	sHaFile := make([]byte, 32)
	_, err = file.Read(sHaFile)
	if err != nil {
		log.Printf("Could not read checksum in new firmware: %v\n", err)
		return
	}

	// Now go to the start and read until last 32 byte
	content := make([]byte, stat.Size() - 32)
	_, err = file.Seek(0, 0)
	if err != nil {
		log.Printf("Could not seek to start in new firmware: %v\n", err)
		return
	}

	_, err = file.Read(content)
	if err != nil {
		log.Printf("Could not read content in new firmware: %v\n", err)
		return
	}

	sha := sha256.New()
	sha.Write(content)
	sHaComp := sha.Sum(nil)[:32]

	if bytes.Compare(sHaFile, sHaComp) != 0 {
		return
	}

	_, err = file.Seek(0,0)
	if err != nil {
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
