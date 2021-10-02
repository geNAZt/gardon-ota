#pragma once

#include "../pump/Pump.hpp"
#include "../../util/Average.hpp"
#include "../Module.h"
#include "../../logger/Logger.hpp"

namespace Module {
    namespace Moisture {
        #define ADC_MAX 4096

        Logger::Logger logger("moisture");

        class Moisture : public Average, public ModuleBase {
            public:
                explicit Moisture(char pin, Pump::Pump* pump) : Average::Average(pin) {
                    this->_pump = pump;
                    this->_wantedValue = 46.0f;
                }

                void loop(unsigned long millis) {
                    Average::loop();
                }

                const char* name() {
                    return "Moisture";
                }

                String metric() {
                    return String("moisture value=" + String(this->value(), 8));
                }

                float value() {
                    return this->_value;
                }

                void onFullAverage( unsigned short int average ) {
                    this->_value = (1.0f - ((float) average / (float) ADC_MAX)) * 100.0f;

                    // We don't do stuff when the pump is on, moisture level will change if water is pumped in
                    if (this->_pump->isOn()) {
                        return;
                    }

                    // Is it dryer than wanted? If so we want to pump as much water in until we reached low watermark
                    if (this->_value < this->_wantedValue) {
                        this->_wantedValue = 57.0f;
                        this->_pump->onFor(5 * 60 * 1000);

                        logger.debug(String("Pumping ... current moisture " + String(this->value(), 5)));
                    } else {
                        this->_wantedValue = 46.0f;
                    }
                }

            private:
                char _pin;
                float _value;
                Pump::Pump* _pump;

                float _wantedValue;
        };
    }
}
