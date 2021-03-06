#pragma once

#include "../Module.h"
#include "../../logger/Logger.hpp"

#include <OneWire.h>
#include <DallasTemperature.h>

namespace Module {
    namespace Temperature {
        Logger::Logger logger("temperature");

        class Temperature : public ModuleBase {
            public:
                explicit Temperature(OneWire* oneWire) {
                    this->_temperatureSensor = new DallasTemperature(oneWire);
                    this->_temperatureSensor->begin();
                    this->_temperatureSensor->setWaitForConversion(false);
                }

                void loop(unsigned long millis) {
                    if (this->_temperatureSensor->isConversionComplete()) {
                        float temp = this->_temperatureSensor->getTempCByIndex(0);
                        if (temp > -127) {
                            this->_lastTemperature = this->_temperatureSensor->getTempCByIndex(0);
                        }
                        
                        this->_temperatureSensor->requestTemperatures();
                    }
                }

                const char* name() {
                    return "Temperature";
                }

                String metric() {
                    return String("temperature value=" + String(this->temperature(), 3));
                }

                float temperature() {
                    return this->_lastTemperature;
                }

            private:
                DallasTemperature* _temperatureSensor;
                float _lastTemperature{};
        };

    }
}
