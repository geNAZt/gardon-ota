#include "OTAPacket.h"

namespace Network { namespace Packet {
    void OTAPacket::read(Buffer* buf) {
        this->_type = (Type) buf->readByte();
        this->_partition = (Partition) buf->readByte();
    }

    size_t OTAPacket::estLength() {
        return 2;
    }

    uint8_t OTAPacket::id() {
        return 0;
    }

    void OTAPacket::write(Buffer* buf) {
        // Only server can send this, client has no write
    }

    OTAPacket::Type OTAPacket::type() {
        return this->_type;
    }

    OTAPacket::Partition OTAPacket::partition() {
        return this->_partition;
    }
}}