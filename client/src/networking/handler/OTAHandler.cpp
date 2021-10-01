#include <Update.h>

#include "OTAHandler.h"

#include "../packet/OTAPacket.h"

namespace Network { 
    namespace Handler {
        Packet::Packet* OTAHandler::generatePacket() {
            return new Packet::OTAPacket();
        }

        void OTAHandler::process(Packet::Packet* packet) {
            Packet::OTAPacket* otaPacket = (Packet::OTAPacket*) packet;

            // Should we start a new OTA patch?
            if (otaPacket->type() == Packet::OTAPacket::Type::START) {
                Serial.printf("Wanting to start OTA update: %i size", otaPacket->size());
                if (!Update.begin(otaPacket->size(), otaPacket->partition() == Packet::OTAPacket::Partition::FIRMWARE ? U_FLASH : U_SPIFFS, -1, LOW, NULL)) {
                    Serial.println("Could not start update ... restarting");
                    ESP.restart();
                }

                Update.setMD5(otaPacket->checksum().c_str());
                Serial.println("Started new OTA update");
            } else if (otaPacket->type() == Packet::OTAPacket::Type::CHUNK) {
                Util::FatPointer ptr = otaPacket->data();
                Update.write(ptr.data, ptr.size);
                Serial.println("Got OTA firmware chunk");
            } else if (otaPacket->type() == Packet::OTAPacket::Type::END) {
                if (Update.end()) {
                    ESP.restart();
                } else {
                    Serial.println("Error whilst OTA update:");
                    Serial.println(Update.errorString());
                }
            }
        }
    }
}