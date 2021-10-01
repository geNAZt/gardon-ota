package packet

import (
	"io"

	"gardon.local/server/util"
)

type Type byte
type Partition byte

const (
	START Type = iota
	CHUNK
	END
)

const (
	FS Partition = iota
	FIRMWARE
)

type OTAPacket struct {
	Packet

	actionType Type
	partition  Partition

	chunkData []byte
	size      uint32
	checksum  string
}

func NewOTAPacket(actionType Type, partition Partition) *OTAPacket {
	return &OTAPacket{
		actionType: actionType,
		partition:  partition,
	}
}

func NewOTAPacketStart(partition Partition, size uint32, checksum string) *OTAPacket {
	return &OTAPacket{
		actionType: START,
		partition:  partition,
		size:       size,
		checksum:   checksum,
	}
}

func NewOTAPacketWithChunk(actionType Type, chunkData []byte) *OTAPacket {
	return &OTAPacket{
		actionType: actionType,
		chunkData:  chunkData,
	}
}

func (pkt *OTAPacket) ID() byte {
	return 1
}

func (pkt *OTAPacket) Write(buf io.Writer) error {
	err := util.WriteByte(buf, byte(pkt.actionType))
	if err != nil {
		return err
	}

	if pkt.actionType == START {
		err = util.WriteByte(buf, byte(pkt.partition))
		if err != nil {
			return err
		}

		err = util.WriteUnsignedInt(buf, pkt.size)
		if err != nil {
			return err
		}

		err = util.WriteString(buf, pkt.checksum)
	} else if pkt.actionType == CHUNK {
		err = util.WriteBytes(buf, pkt.chunkData)
	}

	return err
}

func (pkt *OTAPacket) Read(buf io.Reader) error {
	return nil
}
