#include "LogPacket.h"

namespace Network { 
    namespace Packet {
        LogPacket::LogPacket(const String& message) : _logMessage(message) {

        }

        size_t LogPacket::estLength() {
            return this->_logMessage.length();
        }

        uint8_t LogPacket::id() {
            return 2;
        }

        void LogPacket::write(Buffer& buf) {
            buf.writeString(this->_logMessage);
        }

        void LogPacket::read(Buffer& buf) {
        }
    }
}