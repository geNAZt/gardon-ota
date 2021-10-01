#pragma once

#include "Packet.h"

namespace Network { 
    namespace Packet {
        class LogPacket : public Packet {
            public: 
                uint8_t id() override;
                size_t estLength() override;
                void read(Buffer* buf) override;
                void write(Buffer* buf) override; 

                void setLogMessage(String message);
            private:
                String _logMessage;
        };
    }
}