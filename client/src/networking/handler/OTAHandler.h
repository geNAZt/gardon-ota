#pragma once

#include "PacketHandler.h"

namespace Network { 
    namespace Handler {
        class OTAHandler : public PacketHandler {
            public:
                Packet::Packet* generatePacket();
                void process(Packet::Packet* packet);
        };
    }
}