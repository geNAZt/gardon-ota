
#pragma once

#include <NTPClient.h>
#include "../Module.h"

namespace Module {
    namespace Lamp {
        class Lamp : public ModuleBase {
            public:
                explicit Lamp(char pin, NTPClient* timeClient) {
                    this->_pin = pin;
                    this->_time = timeClient;

                    pinMode(this->_pin, OUTPUT);
                    this->off();
                }

                void loop(unsigned long millis) {
                    if (this->_time->getHours() >= 8 && this->_time->getHours() <= 18) {
                        this->on();
                    } else {
                        this->off();
                    }
                }

                const char* name() {
                    return "Lamp";
                }

                String metric() {
                    return String("lamp value=" + String((this->isOn()) ? "1" : "0"));
                }

                bool isOn() {
                    return this->_on;
                }

            private:
                char _pin;
                NTPClient* _time;

                bool _on{};

                void off() {
                    this->_on = false;
                    digitalWrite(this->_pin, HIGH);
                }

                void on() {
                    this->_on = true;
                    digitalWrite(this->_pin, LOW);
                }
        };
    }
}
