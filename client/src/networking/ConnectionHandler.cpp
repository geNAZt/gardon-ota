#include "Buffer.h"

#include "ConnectionHandler.h"
#include "handler/OTAHandler.h"

#include "packet/LogPacket.h"
#include "packet/CurrentVersionPacket.h"

namespace Network {
    ConnectionHandler::ConnectionHandler(WiFiClientSecure* connection) {
        this->_connection = connection;    

        // Init all handlers
        this->_handlers[1] = new Handler::OTAHandler();
    }

    void ConnectionHandler::remote(String remote) {
        this->_lastReconnect = 0;
        this->_remote = remote;
    }

    void ConnectionHandler::connect() {
        // Connecting to control
        this->_connection->connect(this->_remote.c_str(), 65432, 5000);

        // Check what firmware we currently run
        String hash = ESP.getSketchMD5();
        Serial.print("Current firmware hash ");
        Serial.println(hash);

        // Tell the server which firmware we have so it can decide if we need a new one
        Packet::CurrentVersionPacket* packet = new Packet::CurrentVersionPacket();
        packet->firmwareChecksum(hash);
        this->write(packet);
    }

    void ConnectionHandler::loop() {
        // Check if we are connected
        if (!this->_connection->connected() && this->canReconnect()) {
            this->connect();
            this->_lastReconnect = millis();
        }

        // Check if connected
        if (!this->_connection->connected()) {
            // Release the mutex so that the creating function can finish
            return;
        }

        // Check if there is data
        while (this->_connection->available() > 0) {
            Buffer buf(this->_connection);
            byte packetID = buf.readByte();
            Handler::PacketHandler* handler = this->_handlers[packetID];
            if (handler != nullptr) {
                Packet::Packet* packet = handler->generatePacket();
                packet->read(buf);
                buf.checkEnd();
                handler->process(packet);
                delete packet;
            } else {
                Serial.println("Unknown packet id");
            }
        }
    }

    void ConnectionHandler::write(Packet::Packet* packet) {
        // Only send when connected
        if (!this->_connection->connected()) {
            // We delay and try again
            delay(5000);
            this->write(packet);
            return;
        }

        // Create buffer and write packet into it
        Buffer buf(1 + packet->estLength());
        buf.writeByte(packet->id());
        packet->write(buf);

        // Write packet size first
        uint8_t* packetSizeBytes = (uint8_t*) malloc(2);
        packetSizeBytes[0] = (uint8_t) buf.length() >> 8;
        packetSizeBytes[1] = (uint8_t) buf.length();
        this->_connection->write(packetSizeBytes, 2);
        free(packetSizeBytes);

        // Then write the packet contents
        this->_connection->write(buf.buf(), buf.length());
        delete packet;
    }

    bool ConnectionHandler::canReconnect() {
        return millis() - this->_lastReconnect > 30000;
    }
}