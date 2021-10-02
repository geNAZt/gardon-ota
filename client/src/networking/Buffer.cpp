#include "Buffer.h"

namespace Network {
    Buffer::Buffer(WiFiClientSecure* connection) {
        this->_connection = connection;

        // We always have a short as packet length
        this->_wantedToRead = 2;
        this->_wantedToRead = this->readUnsignedShort() + 2;
    }

    Buffer::Buffer(size_t want) {
        this->_buf = (uint8_t*) malloc(want);
        this->_curSize = want;
    }

    Buffer::~Buffer() {
        if (!this->_connection && this->_buf) {
            free(this->_buf);
        }
    }

    uint8_t* Buffer::buf() {
        return this->_buf;
    }

    size_t Buffer::length() {
        return this->_pos;
    }

    void Buffer::_ensureWritable(size_t count) {
        if (this->_curSize < this->_pos + count) {
            uint8_t* newBuf = (uint8_t*) realloc(this->_buf, this->_curSize + count);
            if (newBuf != NULL) {
                this->_buf = newBuf;
                this->_curSize = this->_curSize + count;
                return;
            }

            throw std::out_of_range("can't write, no memory left");
        }
    }

    void Buffer::_ensureReadable(size_t count) {
        if (this->_wantedToRead < this->_pos + count) {
            Serial.println("can't read beyond packet length");
            throw std::out_of_range("can't read beyond packet length");
        }
    }

    unsigned short Buffer::readUnsignedShort() {
        this->_ensureReadable(2);
        uint8_t* bufData = (uint8_t*) malloc(2);
        this->_connection->readBytes(bufData, 2);
        unsigned short uShort = (bufData[0] << 8) | bufData[1];
        free(bufData);
        this->_pos += 2;
        return uShort;
    }

    void Buffer::writeUnsignedShort(unsigned short value) {
        this->_ensureWritable(2);
        this->_buf[this->_pos++] = (value >> 8) & 0xFF; 
        this->_buf[this->_pos++] = value & 0xFF;
    }

    String Buffer::readString() {
        Util::FatPointer pointer = this->readBytes();

        String str;
        str.reserve(pointer.size + 1);
        str = (char*) pointer.data;
        free(pointer.data);
        return str;        
    }

    Util::FatPointer Buffer::readBytes() {
        unsigned short len = this->readUnsignedShort();
        this->_ensureReadable(len);

        uint8_t* data = (uint8_t*) malloc(len);
        this->_connection->readBytes(data, len);
        this->_pos += len;

        return Util::FatPointer{
            .data = data,
            .size = len
        };        
    }

    void Buffer::writeString(const String& str) {
        this->writeUnsignedShort(str.length());
        this->_ensureWritable(str.length());
        strncpy((char*) (this->_buf + this->_pos), str.c_str(), str.length());
        this->_pos += str.length();
    }

    byte Buffer::readByte() {
        this->_ensureReadable(1);
        uint8_t* bufData = (uint8_t*) malloc(1);
        this->_connection->readBytes(bufData, 1);
        byte data = bufData[0] + 0;
        free(bufData);
        this->_pos++;
        return data;
    }
    
    void Buffer::writeByte(byte value) {
        this->_ensureWritable(1);
        this->_buf[this->_pos++] = value;
    }

    void Buffer::writeBytes(Util::FatPointer value) {
        this->writeUnsignedShort(value.size);
        this->_ensureWritable(value.size);
        memcpy(this->_buf + this->_pos, value.data, value.size);
        this->_pos += value.size;
    }

    void Buffer::writeUnsignedInt(unsigned int value) {
        this->_ensureWritable(4);
        this->_buf[this->_pos++] = (value >> 24) & 0xFF; 
        this->_buf[this->_pos++] = (value >> 16) & 0xFF; 
        this->_buf[this->_pos++] = (value >> 8) & 0xFF; 
        this->_buf[this->_pos++] = value & 0xFF;
    }

    unsigned int Buffer::readUnsignedInt() {
        this->_ensureReadable(4);
        uint8_t* bufData = (uint8_t*) malloc(4);
        this->_connection->readBytes(bufData, 4);
        unsigned int uInt = (bufData[0] << 24) | (bufData[1] << 16) | (bufData[2] << 8) | bufData[3];
        free(bufData);
        this->_pos += 4;
        return uInt;
    }

    void Buffer::print() {
        for (int i = 0; i < this->_pos; i++) {
            Serial.print(this->_buf[i]);
            Serial.print(" ");
        }

        Serial.println();
    }

    void Buffer::checkEnd() {
        size_t left = (this->_wantedToRead - this->_pos);
        if (left > 0) {
            Serial.printf("Remaining bytes: %i\n", left);
        }
    }

}