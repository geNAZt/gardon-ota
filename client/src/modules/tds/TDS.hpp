#pragma once

#include "../temperature/Temperature.hpp"
#include "../../util/Average.hpp"
#include "../Module.h"

namespace Module {
    namespace TDS {
        #define ADC_MAX 4096

        class TDS : public Average, public ModuleBase {
            public:
                explicit TDS(char pin, Temperature::Temperature* temperature) : Average::Average(pin) {
                    this->_temperature = temperature;
                }

                void loop(unsigned long millis) {
                    Average::loop();
                }

                const char* name() {
                    return "TDS";
                }

                String metric() {
                    return String("tds value=" + String(this->ppmValue(), 3));
                }

                float ppmValue() {
                    return this->_ppmValue;
                }

                void onFullAverage( unsigned short int average ) {
                    float voltage = (((float) average) * 3.3f) / ADC_MAX;
                    float compensationCoefficient = 1.0 + 0.02 * (this->_temperature->temperature() - 25.0f);
                    float compensationVoltage = voltage / compensationCoefficient;

                    float tempPPMValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
                    this->_ppmValue = tempPPMValue;
                }

            private:
                Temperature::Temperature* _temperature;

                float _ppmValue{};
        };

    }
}
