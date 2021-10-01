#pragma once

#include "Packet.h"

namespace Network { 
    namespace Packet {
        class CurrentVersionPacket : public Packet {
            public:
                uint8_t id() override;
                size_t estLength() override;
                void read(Buffer* buf) override;
                void write(Buffer* buf) override; 

                String firmwareChecksum();
                void firmwareChecksum(String firmwareChecksum);
            private:
                String _firmwareChecksum;
        };
    }
}