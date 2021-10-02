#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <stdexcept>

#include "../util/FatPointer.hpp"

namespace Network {
    class Buffer {
        public:
            Buffer(WiFiClientSecure* connection);
            Buffer(size_t size);
            ~Buffer();

            unsigned short readUnsignedShort();
            void writeUnsignedShort(unsigned short value);

            String readString();
            void writeString(const String& value);

            byte readByte();
            void writeByte(byte value);

            Util::FatPointer readBytes();
            void writeBytes(Util::FatPointer value);

            unsigned int readUnsignedInt();
            void writeUnsignedInt(unsigned int value);

            size_t length();
            uint8_t* buf();

            void print();
            void checkEnd();
        private:
            unsigned short _pos{};

            WiFiClientSecure* _connection{};
            unsigned short _wantedToRead{};

            uint8_t* _buf{};
            size_t _curSize;

            void _ensureWritable(size_t count);
            void _ensureReadable(size_t count);
    };
}