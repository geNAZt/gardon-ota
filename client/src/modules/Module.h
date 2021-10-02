#pragma once

#include <Arduino.h>

namespace Module {
    class ModuleBase {
        public:
            virtual void loop(unsigned long millis) = 0;
            virtual const char* name() = 0;
            virtual String metric() = 0;
    };
}

