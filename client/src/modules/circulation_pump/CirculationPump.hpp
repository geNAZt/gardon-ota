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

                    // Setup PWM
                    ledcSetup(0, 30000, 8);
                    ledcAttachPin(this->_pin, 0);

                    this->off();
                }

                void loop(unsigned long millis) {
                    if (this->_onUntil != 0 && millis >= this->_onUntil) {
                        this->off();
                    }

                    // Time to start table
                    if ((this->_time->getMinutes() == 0 || this->_time->getMinutes() == 30) && this->_time->getSeconds() == 0) {
                        this->onFor(30 * 1000);
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
                    ledcWrite(this->_pin, 85);
                    this->_onUntil = millis() + forMillis;
                }

                void off() {
                    this->_on = false;
                    ledcWrite(this->_pin, 0);
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
