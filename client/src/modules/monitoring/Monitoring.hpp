#pragma once

#include <WiFi.h>

#include "../Module.h"

namespace Module {
    namespace Monitoring {
        class Monitoring : public ModuleBase {
            public:
                explicit Monitoring(ModuleBase** modules, unsigned char moduleAmount) {
                    this->_modules = modules;
                    this->_moduleAmount = moduleAmount;
                    this->_udp.begin(1338);
                }

                void loop(unsigned long millis) {
                    if (this->_index == 99) {
                        this->_index = 0;

                        for (unsigned char i = 0; i < this->_moduleAmount; i++) {
                            ModuleBase* module = this->_modules[i];
                            String metric = module->metric();
                            if (metric.length() > 0) {
                                this->_udp.beginPacket("192.168.0.187", 8888);
                                this->_udp.print(metric);
                                this->_udp.endPacket();
                            }
                        }
                    } else {
                        this->_index++;
                    }
                }

                const char* name() {
                    return "Monitoring";
                }

                String metric() {
                    return String("");
                }

            private:
                WiFiUDP _udp;
                ModuleBase** _modules;
                unsigned char _moduleAmount;

                unsigned char _index{};
        };
    }
}
