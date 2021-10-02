#pragma once

#include "Packet.h"

namespace Network { 
    namespace Packet {
        class LogPacket : public Packet {
            public: 
                LogPacket(const String& message);
                uint8_t id() override;
                size_t estLength() override;
                void read(Buffer& buf) override;
                void write(Buffer& buf) override; 
            private:
                const String& _logMessage;
        };
    }
}