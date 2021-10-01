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

                ~OTAPacket();

                uint8_t id() override;
                size_t estLength() override;
                void read(Buffer* buf) override;
                void write(Buffer* buf) override; 

                Type type();
                Partition partition();
                Util::FatPointer data();
                unsigned int size();
                String checksum();
            private:
                Type _type;
                Partition _partition;
                Util::FatPointer _data;
                unsigned int _size;
                String _checksum;
        };
    }
}