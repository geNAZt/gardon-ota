#pragma once

#include <NTPClient.h>

#include "../Module.h"

namespace Module {
    namespace AirStone {
        class AirStone : public ModuleBase {
            public:
                explicit AirStone(char pin, NTPClient* timeClient) {
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
                        this->onFor(5 * 1000);
                    }
                }

                const char* name() {
                    return "AirStone";
                }

                String metric() {
                    return String("airstone value=" + String((this->isOn()) ? "1" : "0"));
                }

                void onFor(unsigned long forMillis) {
                    digitalWrite(this->_pin, HIGH);
                    this->_on = true;
                    this->_onUntil = millis() + forMillis;
                }

                void off() {
                    digitalWrite(this->_pin, LOW);
                    this->_on = false;
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
