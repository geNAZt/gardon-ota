#include "Buffer.h"

#include "ConnectionHandler.h"
#include "handler/OTAHandler.h"

namespace Network {
    ConnectionHandler::ConnectionHandler(WiFiClientSecure* connection) {
        this->_connection = connection;    

        // Init all handlers
        this->_handlers[1] = new Handler::OTAHandler();
    }

    void ConnectionHandler::loop() {
        if (this->_connection->available() > 0) {
            Buffer buf(this->_connection);
            byte packetID = buf.readByte();
            Handler::PacketHandler* handler = this->_handlers[packetID];
            if (handler != nullptr) {
                Packet::Packet* packet = handler->generatePacket();
                packet->read(&buf);
                handler->process(packet);
                free(packet);
            } else {
                throw std::invalid_argument("Unknown packet id");
            }
        }
    }

    void ConnectionHandler::write(Packet::Packet* packet) {
        // Create buffer and write packet into it
        Buffer buf(1 + packet->estLength());
        buf.writeByte(packet->id());
        packet->write(&buf);

        // Write packet size first
        uint8_t* packetSizeBytes = (uint8_t*) malloc(2);
        packetSizeBytes[0] = (uint8_t) buf.length() >> 8;
        packetSizeBytes[1] = (uint8_t) buf.length();
        this->_connection->write(packetSizeBytes, 2);
        free(packetSizeBytes);

        // Then write the packet contents
        this->_connection->write(buf.buf(), buf.length());
    }
}