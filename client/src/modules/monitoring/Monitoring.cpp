#include "Monitoring.h"
#include "../../util/Debug.h"
#include <Arduino.h>

Monitoring::Monitoring(Module** modules, unsigned char moduleAmount) {
    this->_modules = modules;
    this->_moduleAmount = moduleAmount;
    this->_udp.begin(1338);
}

void Monitoring::loop(unsigned long millis) {
    if (this->_index == 99) {
        this->_index = 0;

        for (unsigned char i = 0; i < this->_moduleAmount; i++) {
            Module* module = this->_modules[i];
            String metric = module->metric();
            if (metric.length() > 0) {
                this->_udp.beginPacket("192.168.0.187", 8888);
                this->_udp.print(metric);
                this->_udp.endPacket();
            }
        }
    } else {
        this->_index++;
    }
}

const char* Monitoring::name() {
  return "Monitoring";
}

String Monitoring::metric() {
    return String("");
}