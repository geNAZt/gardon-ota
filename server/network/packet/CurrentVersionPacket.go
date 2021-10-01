package packet

import (
	"gardon.local/server/util"
	"io"
)

type CurrentVersionPacket struct {
	Packet
	firmwareChecksum string
}

func (pkt *CurrentVersionPacket) ID() byte {
	return 0
}

func (pkt *CurrentVersionPacket) Write(buf io.Writer) error {
	return nil
}

func (pkt *CurrentVersionPacket) Read(buf io.Reader) error {
	readString, err := util.ReadString(buf)
	if err != nil {
		return err
	}

	pkt.firmwareChecksum = readString
	return nil
}

func (pkt *CurrentVersionPacket) FirmwareChecksum() string {
	return pkt.firmwareChecksum
}