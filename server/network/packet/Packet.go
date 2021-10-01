package packet

import (
	"io"
)

type Packet interface {
	ID() byte
	Write(buf io.Writer) error
	Read(buf io.Reader) error
}
