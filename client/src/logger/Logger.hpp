#pragma once

#include "../networking/ConnectionHandler.h"
#include "../networking/packet/LogPacket.h"

namespace Logger {
    Network::ConnectionHandler* _connectionHandler;    
    
    void init(Network::ConnectionHandler* connectionHandler) {
        _connectionHandler = connectionHandler;
    }

    class Logger {
        private:
            String _name;

            void send(const String& level, const String& message) {
                String finalMessage = this->_name + F("[") + level + F("] ") + message;
                Network::Packet::LogPacket* pkt = new Network::Packet::LogPacket((const String&) finalMessage);
                _connectionHandler->write(pkt);
            }
        public:
            Logger(String name) {
                this->_name = name;
            }

            void debug(const String& message) {
                this->send(F("DEBUG"), message);
            }

            void warn(const String& message) {
                this->send(F("WARN"), message);
            }

            void info(const String& message) {
                this->send(F("INFO"), message);
            }
    };
}