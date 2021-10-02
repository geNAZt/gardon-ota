package handler

import (
	"gardon.local/server/client"
	"gardon.local/server/network/packet"
	"log"
)

type LogHandler struct {
	Handler
}

func (l *LogHandler) GeneratePacket() packet.Packet {
	return &packet.LogPacket{}
}

func (l *LogHandler) Process(pkt packet.Packet, client client.Client) {
	log.Printf("[%s] %s", client.Name(), pkt.(*packet.LogPacket).Message)
}
