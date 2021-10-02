#include "CurrentVersionPacket.h"

namespace Network { 
    namespace Packet {
        uint8_t CurrentVersionPacket::id() {
            return 0;
        }

        size_t CurrentVersionPacket::estLength() {    
            return 2 + this->_firmwareChecksum.length();
        }

        void CurrentVersionPacket::read(Buffer& buffer) {

        }

        void CurrentVersionPacket::write(Buffer& buffer) {
            buffer.writeString(this->_firmwareChecksum);
        }

        String CurrentVersionPacket::firmwareChecksum() {
            return this->_firmwareChecksum;
        }

        void CurrentVersionPacket::firmwareChecksum(String firmwareChecksum) {
            this->_firmwareChecksum = firmwareChecksum;
        }
    }
}