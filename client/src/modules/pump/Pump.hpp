#pragma once

#include <NTPClient.h>

#include "../Module.h"

namespace Module {
    namespace Pump {
        class Pump : public ModuleBase {
            public:
                explicit Pump(char pin, NTPClient* timeClient) {
                    this->_pin = pin;
                    this->_time = timeClient;

                    pinMode(this->_pin, OUTPUT);
                    this->off();
                }

                void loop(unsigned long millis) {
                    if (this->_onUntil != 0 && millis >= this->_onUntil) {
                        this->off();
                    }
                }

                const char* name() {
                    return "Pump";
                }

                String metric() {
                    return String("pump value=" + String((this->isOn()) ? "1" : "0"));
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
