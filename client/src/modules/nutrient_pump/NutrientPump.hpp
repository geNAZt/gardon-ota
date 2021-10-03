#pragma once

#include "../tds/TDS.hpp"
#include "../circulation_pump/CirculationPump.hpp"
#include "../Module.h"
#include "../../logger/Logger.hpp"

namespace Module {
    namespace NutrientPump {
        Logger::Logger logger("nutrientPump");

        class NutrientPump : public ModuleBase {
            public:
                explicit NutrientPump(char pin, TDS::TDS* tds, CirculationPump::CirculationPump* circulationPump) {
                    this->_pin = pin;
                    this->_tds = tds;
                    this->_circulationPump = circulationPump;

                    pinMode(this->_pin, OUTPUT);
                    this->off();
                }

                void loop(unsigned long millis) {
                    if (this->_pumpUntil != 0 && millis >= this->_pumpUntil) {
                        this->off();
                    }

                    // We don't pump during circulation
                    if (this->_circulationPump->isOn()) {
                        return;
                    }

                    if (this->_inProgressUntil != 0 && millis >= this->_inProgressUntil) {
                        this->_inProgressUntil = 0;
                    }

                    if (this->_inProgressUntil == 0) {
                        float ppmValue = this->_tds->ppmValue();
                        
                        // First check if we need to down pH
                        if ( ppmValue < 950 ) {
                            this->_occurence++;
                            if (this->_occurence == 5) {
                                logger.info("Pumping nutrients...");
                                this->on();
                            }
                        } else {
                            this->_occurence = 0;
                        }
                    }
                }

                const char* name() {
                    return "NutrientPump";
                }

                String metric() {
                    return String("nutrientpump value=" + String((this->isOn()) ? "1" : "0"));
                }

                bool isOn() {
                    return this->_on;
                }

            private:
                TDS::TDS* _tds;
                CirculationPump::CirculationPump* _circulationPump;

                unsigned long _inProgressUntil{};
                unsigned long _pumpUntil{};

                char _pin, _occurence{};
                bool _on{};

                void off() {
                    digitalWrite(this->_pin, HIGH);
                    this->_pumpUntil = 0;
                    this->_on = false;
                }

                void on() {
                    digitalWrite(this->_pin, LOW);
                    this->_pumpUntil = millis() + 2000;
                    this->_inProgressUntil = millis() + 30000;
                    this->_circulationPump->onFor(10000);
                    this->_on = true;
                }
            };
    }
}
