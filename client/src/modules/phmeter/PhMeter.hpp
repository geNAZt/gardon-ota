#pragma once

#include "../Module.h"
#include "../temperature/Temperature.hpp"
#include "../../util/Average.hpp"
#include "../../logger/Logger.hpp"

namespace Module {
    namespace PhMeter {
        Logger::Logger logger("phMeter");

        #define ADC_MAX 4096
        #define PH_PER_C 0.02f

        float VOLT_FOR_4_PH = 1.86994624f;
        float VOLT_FOR_7_PH = 1.35029292f;

        float m = 3 / (VOLT_FOR_7_PH - VOLT_FOR_4_PH);
        float b = 4 - (VOLT_FOR_4_PH * m);

        class PhMeter : public Average, public ModuleBase {
            public:
                explicit PhMeter(char pin, Temperature::Temperature* temperature) : Average::Average(pin) {
                    this->_temperature = temperature;
                }

                void loop(unsigned long millis) {
                    Average::loop();
                }

                const char* name() {
                    return "PhMeter";
                }

                String metric() {
                    return String("ph value=" + String(this->phValue(), 3));
                }

                float phValue() {
                    return this->_phValue;
                }

                void onFullAverage( unsigned short int average ) {
                    float voltage = ((float) average * 3.3f) / ADC_MAX;
                    // logger.debug(String("Voltage: ") + String(voltage, 8));

                    float tempPhValue = m * voltage + b;
                    // logger.debug(String("PH (before temp): ") + String(tempPhValue, 8));
                    
                    float tempC = this->_temperature->temperature();
                    float diffC = tempC - 25.0f;

                    this->_phValue = tempPhValue + (diffC * PH_PER_C);
                }

            private:
                Temperature::Temperature* _temperature;

                float _phValue{};
            };
    }
}