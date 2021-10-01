#include "OTAPacket.h"

namespace Network { namespace Packet {
    OTAPacket::~OTAPacket() {
        if (this->type() == CHUNK) {
            free(this->_data.data);
        }
    }

    void OTAPacket::read(Buffer* buf) {
        this->_type = (Type) buf->readByte();
        if (this->type() == START) {
            this->_partition = (Partition) buf->readByte();
            this->_size = buf->readUnsignedInt();
            this->_checksum = buf->readString();
        } else if (this->type() == CHUNK) {
            this->_data = buf->readBytes();
        } 
    }

    size_t OTAPacket::estLength() {
        if (this->_type == START) {
            return 2;
        } else if (this->_type == CHUNK) {
            return 1 + this->_data.size;
        }

        return 1;
    }

    uint8_t OTAPacket::id() {
        return 1;
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

    Util::FatPointer OTAPacket::data() {
        return this->_data;
    }

    unsigned int OTAPacket::size() {
        return this->_size;
    }

    String OTAPacket::checksum() {
        return this->_checksum;
    }
}}