#pragma once

#include <Adafruit_INA219.h>

#include "../Module.h"

namespace Module {
    namespace Shunt {
        class Shunt : public ModuleBase {
            public:
                explicit Shunt() {
                    this->_ina216 = new Adafruit_INA219();
                    this->_ina216->begin();
                }

                void loop(unsigned long millis) {

                }

                const char* name() {
                    return "Shunt";
                }

                String metric() {
                    return String("shunt value=" + String(this->_ina216->getPower_mW(), 2));
                }

            private:
                float _milliWatt;
                Adafruit_INA219* _ina216;
        };
    }
}
