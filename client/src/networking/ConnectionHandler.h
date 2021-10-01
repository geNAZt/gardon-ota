#pragma once

#include <WiFiClientSecure.h>

#include "handler/PacketHandler.h"

namespace Network {
    class ConnectionHandler {
        public:
            ConnectionHandler(WiFiClientSecure* connection);

            void loop();
            void write(Packet::Packet* packet);

        private:
            WiFiClientSecure* _connection;
            Handler::PacketHandler* _handlers[2];
    };
}