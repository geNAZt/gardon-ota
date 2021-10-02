#include "LogPacket.h"

namespace Network { 
    namespace Packet {
        void LogPacket::logMessage(String message) {
            this->_logMessage = message;
        }

        size_t LogPacket::estLength() {
            return this->_logMessage.length();
        }

        uint8_t LogPacket::id() {
            return 2;
        }

        void LogPacket::write(Buffer* buf) {
            buf->writeString(this->_logMessage);
        }

        void LogPacket::read(Buffer* buf) {
        }
    }
}