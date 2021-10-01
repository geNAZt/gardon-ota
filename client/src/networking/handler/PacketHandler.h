#pragma once

#include "../packet/Packet.h"

namespace Network { 
    namespace Handler {
        class PacketHandler {
            public:
                virtual Packet::Packet* generatePacket() = 0;
                virtual void process(Packet::Packet* packet) = 0;
        };
    } 
}