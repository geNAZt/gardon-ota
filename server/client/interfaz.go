package client

import "gardon.local/server/network/packet"

type Client interface {
	UpdateFirmware(checksum string)
	SetFirmwareChecksum(checksum string)
	Write(packet packet.Packet)
	Name() string
}
