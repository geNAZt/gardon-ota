package main

import (
	"crypto/rand"
	"crypto/tls"
	"crypto/x509"
	"gardon.local/server/firmware"
	"gardon.local/server/network"
	"io/ioutil"
	"log"
	"net"
	"sync"
)

var connections sync.Map

func main() {
	// Init all needed modules for traffic handling
	fw := firmware.NewFirmware(func(newChecksum string) {
		log.Printf("Got a new firmware checksum: %s\n", newChecksum)
		connections.Range(func(key, value interface{}) bool {
			h := value.(*network.Handler)
			h.UpdateFirmware(newChecksum)
			return true
		})
	})

	network.Init(fw)

	// Load the TLS cert and CAs
	cert, err := tls.LoadX509KeyPair("../pki/server.crt", "../pki/server.key")
	if err != nil {
		log.Fatalf("server: loadkeys: %s", err)
	}

	// Load the client cert CA
	data, err := ioutil.ReadFile("../pki/ca/ca.crt")
	clientCAs := x509.NewCertPool()
	clientCAs.AppendCertsFromPEM(data)

	config := tls.Config{
		Certificates: []tls.Certificate{cert},
		ClientCAs: clientCAs,
	}
	config.Rand = rand.Reader

	// Start the network listener so we can accept clients
	service := "0.0.0.0:65432"
	listener, err := tls.Listen("tcp", service, &config)
	if err != nil {
		log.Fatalf("server: listen: %s", err)
	}

	log.Print("server: listening")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("server: accept: %s", err)
			break
		}

		tlscon, ok := conn.(*tls.Conn)
		if ok {
			log.Printf("Accepted new connection from %s\n", conn.RemoteAddr().String())

			// Do we have an old connection which we need to drop (only one connection per IP is allowed, there is no multiplexing of apps on ESP32)
			if conn, ok := connections.Load(conn.RemoteAddr().String()); ok {
				_ = conn.(net.Conn).Close()
			}

			// Store and delete connections from map
			connections.Store(conn.RemoteAddr().String(), network.NewHandler(tlscon, func(c net.Conn) {
				connections.Delete(c.RemoteAddr().String())
			}))
			// handler.Write(packet.NewOTAPacket(packet.START, packet.FS))
		} else {
			err := conn.Close()
			if err != nil {
				log.Printf("Could not close invalid connection from %s\n", conn.RemoteAddr().String())
			}
		}
	}
}
