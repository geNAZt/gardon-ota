#pragma once

#include "../Buffer.h"

namespace Network { namespace Packet {
    class Packet {
        public:
            virtual uint8_t id() = 0;
            virtual void read(Buffer* buf) = 0;
            virtual void write(Buffer* buf) = 0;
            virtual size_t estLength() = 0;
    };
} }