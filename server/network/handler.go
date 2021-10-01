package network

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"net"

	"gardon.local/server/client"
	"gardon.local/server/firmware"
	"gardon.local/server/network/handler"
	"gardon.local/server/network/packet"
	"gardon.local/server/util"
)

var (
	handlers []handler.Handler
	fw       *firmware.Firmware
)

func Init(firmware *firmware.Firmware) {
	fw = firmware

	handlers = make([]handler.Handler, 1)
	handlers[0] = &handler.CurrentVersionHandler{
		Firmware: fw,
	}
}

type Handler struct {
	conn net.Conn

	data    *client.Data
	closeCB func(conn net.Conn)
}

func NewHandler(conn net.Conn, closeCB func(c net.Conn)) *Handler {
	h := &Handler{
		conn:    conn,
		data:    &client.Data{},
		closeCB: closeCB,
	}
	go h.serve()
	return h
}

func (h *Handler) Write(packet packet.Packet) {
	buf := &bytes.Buffer{}
	buf.WriteByte(packet.ID())
	packet.Write(buf)

	// Write the length of the packet first
	length := make([]byte, 2)
	binary.BigEndian.PutUint16(length, uint16(buf.Len()))
	h.conn.Write(length)

	// Write the contents
	h.conn.Write(buf.Bytes())

	// Debug
	fmt.Print(uint16(buf.Len()))
	fmt.Print(": ")

	if buf.Len() < 100 {
		fmt.Println(buf.Bytes())
	} else {
		fmt.Println(buf.Bytes()[:100])
	}
}

func (h *Handler) serve() {
	defer h.conn.Close()

	for {
		log.Print("server: conn: waiting")
		_, err := util.ReadUnsignedShort(h.conn) // We need some sort of reader which can count
		if err != nil {
			log.Printf("Connection closed: %s", err)
			break
		}

		pktId, err := util.ReadByte(h.conn)
		if err != nil {
			log.Printf("Connection closed: %s", err)
			break
		}

		han := handlers[pktId]
		if han != nil {
			pkt := han.GeneratePacket()
			err := pkt.Read(h.conn)
			if err != nil {
				log.Printf("Connection closed: %s", err)
				break
			}

			han.Process(pkt, h)
		}
	}

	log.Println("server: conn: closed")
	h.closeCB(h.conn)
}

func (h *Handler) UpdateFirmware(checksum string) {
	if h.data.FirmwareVersion != checksum {
		fw.SendUpdate(h)
	}
}

func (h *Handler) SetFirmwareChecksum(checksum string) {
	h.data.FirmwareVersion = checksum
}
