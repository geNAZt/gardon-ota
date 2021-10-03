#pragma once

#include <NTPClient.h>

#include "../Module.h"

namespace Module {
    namespace CirculationPump {
        class CirculationPump : public ModuleBase {
            public:
                explicit CirculationPump(char pin, NTPClient* timeClient) {
                    this->_pin = pin;
                    this->_time = timeClient;

                    pinMode(this->_pin, OUTPUT);

                    this->off();
                }

                void loop(unsigned long millis) {
                    if (this->_onUntil != 0 && millis >= this->_onUntil) {
                        this->off();
                    }

                    // Time to start table
                    if ((this->_time->getMinutes() == 0 || this->_time->getMinutes() == 30) && this->_time->getSeconds() == 0) {
                        this->onFor(5000);
                    }
                }

                const char* name() {
                    return "CirculationPump";
                }

                String metric() {
                    return String("circulationpump value=" + String((this->isOn()) ? "1" : "0"));
                }

                void onFor(unsigned long forMillis) {
                    this->_on = true;
                    digitalWrite(this->_pin, HIGH);
                    this->_onUntil = millis() + forMillis;
                }

                void off() {
                    this->_on = false;
                    digitalWrite(this->_pin, LOW);
                }

                bool isOn() {
                    return this->_on;
                }

            private:
                char _pin;
                
                unsigned long _onUntil{};
                NTPClient* _time;

                bool _on{};
        };
    }
}
