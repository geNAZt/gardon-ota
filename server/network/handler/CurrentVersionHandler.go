package handler

import (
	"gardon.local/server/client"
	"gardon.local/server/firmware"
	"gardon.local/server/network/packet"
	"log"
)

type CurrentVersionHandler struct {
	Handler
	Firmware *firmware.Firmware
}

func (h *CurrentVersionHandler) GeneratePacket() packet.Packet {
	return &packet.CurrentVersionPacket{}
}

func (h *CurrentVersionHandler) Process(pack packet.Packet, cl client.Client) {
	pkt := pack.(*packet.CurrentVersionPacket)
	log.Printf("New client with firmware: %s", pkt.FirmwareChecksum())
	cl.SetFirmwareChecksum(pkt.FirmwareChecksum())
	cl.UpdateFirmware(h.Firmware.Checksum())
}