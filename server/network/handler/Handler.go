package handler

import (
	"gardon.local/server/client"
	"gardon.local/server/network/packet"
)

type Handler interface {
	GeneratePacket() packet.Packet
	Process(packet packet.Packet, data client.Client)
}
