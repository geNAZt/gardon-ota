#include "OTAHandler.h"

#include "../packet/OTAPacket.h"

namespace Network { 
    namespace Handler {
        Packet::Packet* OTAHandler::generatePacket() {
            return new Packet::OTAPacket();
        }

        void OTAHandler::process(Packet::Packet* packet) {
            Packet::OTAPacket* otaPacket = (Packet::OTAPacket*) packet;

            Serial.print("Got OTA update: ");
            Serial.print("Type: ");
            Serial.print(otaPacket->type());
            Serial.print("Partition: ");
            Serial.println(otaPacket->partition());
        }
    }
}