#pragma once

#include "Packet.h"

namespace Network { 
    namespace Packet {
        class OTAPacket : public Packet {
            public:
                enum Type {
                    START,
                    CHUNK,
                    END
                };

                enum Partition {
                    FS,
                    FIRMWARE
                };

                uint8_t id() override;
                size_t estLength() override;
                void read(Buffer* buf) override;
                void write(Buffer* buf) override; 

                Type type();
                Partition partition();
            private:
                Type _type;
                Partition _partition;
        };
    }
}