#pragma once

#include "../phmeter/PhMeter.hpp"
#include "../circulation_pump/CirculationPump.hpp"
#include "../Module.h"
#include "../../logger/Logger.hpp"

namespace Module {
    namespace PhUpDown {
        #define PHUPDOWN_DOWN_LIMIT 7.2f
        #define PHUPDOWN_DOWN_REACH 6.5f

        #define PHUPDOWN_UP_LIMIT 5.8f
        #define PHUPDOWN_UP_REACH 6.5f

        Logger::Logger logger("phUpDown");

        class PhUpDown : public ModuleBase {
            public:
                explicit PhUpDown(char upPin, char downPin, PhMeter::PhMeter* phMeter, CirculationPump::CirculationPump* circulationPump) {
                    this->_upPin = upPin;
                    this->_downPin = downPin;
                    this->_phMeter = phMeter;
                    this->_circulationPump = circulationPump;

                    pinMode(this->_upPin, OUTPUT);
                    pinMode(this->_downPin, OUTPUT);
                }

                void loop(unsigned long millis) {
                    if (this->_pumpUntil != 0 && millis >= this->_pumpUntil) {
                        this->allOff();
                    }

                    // We don't pump during circulation
                    if (this->_circulationPump->isOn()) {
                        return;
                    }

                    if (this->_inProgressUntil != 0 && millis >= this->_inProgressUntil) {
                        this->_inProgressUntil = 0;
                    }

                    if (this->_inProgressUntil == 0) {
                        float phValue = this->_phMeter->phValue();
                        if (phValue > 0.1) {
                            // Do we have reach targets?
                            if (this->_downReachTarget > 0.1f) {
                                logger.debug("Pumpin for ph down ... target " + String(this->_downReachTarget, 2) + ", current " + String(phValue, 2));
                                if (phValue >= this->_downReachTarget) {
                                    this->down();
                                } else {
                                    this->_downReachTarget = 0.0f;
                                }
                            } else  if (this->_upReachTarget > 0.1f) {
                                logger.debug("Pumpin for ph up ... target " + String(this->_upReachTarget, 2) + ", current " + String(phValue, 2));
                                if (phValue <= this->_upReachTarget) {
                                    this->up();
                                } else {
                                    this->_upReachTarget = 0.0f;
                                }
                            } else {
                                // First check if we need to down pH
                                if (phValue >= PHUPDOWN_DOWN_LIMIT) {
                                    this->_occurence++;
                                    if (this->_occurence == 5) {
                                        logger.debug("Pumpin for ph down...");
                                        this->down();
                                        this->_downReachTarget = PHUPDOWN_DOWN_REACH;
                                        this->_occurence = 0;
                                    } else {
                                        this->_circulationPump->onFor(1000);
                                    }
                                } else if (phValue <= PHUPDOWN_UP_LIMIT) {
                                    this->_occurence++;
                                    if (this->_occurence == 5) {
                                        logger.debug("Pumpin for ph up...");
                                        this->up();
                                        this->_upReachTarget = PHUPDOWN_UP_REACH;
                                        this->_occurence = 0;
                                    } else {
                                        this->_circulationPump->onFor(1000);
                                    }
                                } else {
                                    this->_occurence = 0;
                                }
                            }
                        }
                    }
                }

                const char* name() {
                    return "PhUpDown";
                }

                String metric() {
                    return String("phupdown value=" + String(this->_direction, 10));
                }

            private:
                PhMeter::PhMeter* _phMeter;
                CirculationPump::CirculationPump* _circulationPump;

                float _downReachTarget{}, _upReachTarget{};

                unsigned long _inProgressUntil{};
                unsigned long _pumpUntil{};

                char _upPin;
                char _downPin;

                char _direction{}, _occurence{};

                void allOff() {
                    digitalWrite(this->_upPin, LOW);
                    digitalWrite(this->_downPin, LOW);
                    this->_pumpUntil = 0;
                    this->_direction = 0;
                }

                void down() {
                    digitalWrite(this->_downPin, HIGH);
                    this->_direction = -1;
                    this->_pumpUntil = millis() + 500;
                    this->_inProgressUntil = millis() + 60 * 1000;
                    this->_circulationPump->onFor(30 * 1000);
                }

                void up() {
                    digitalWrite(this->_upPin, HIGH);
                    this->_direction = 1;
                    this->_pumpUntil = millis() + (5 * 200);
                    this->_inProgressUntil = millis() + 60 * 1000;
                    this->_circulationPump->onFor(30 * 1000);
                }
        };
    }
}
