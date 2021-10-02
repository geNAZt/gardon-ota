package packet

import (
	"gardon.local/server/util"
	"io"
)

type LogPacket struct {
	Message string
}

func (pkt *LogPacket) ID() byte {
	return 2
}

func (pkt *LogPacket) Write(buf io.Writer) error {
	err := util.WriteString(buf, pkt.Message)
	return err
}

func (pkt *LogPacket) Read(buf io.Reader) error {
	readString, err := util.ReadString(buf)
	pkt.Message = readString
	return err
}
