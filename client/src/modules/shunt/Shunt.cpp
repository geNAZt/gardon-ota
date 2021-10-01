#include <Arduino.h>
#include "Shunt.h"

Shunt::Shunt() {
    this->_ina216 = new Adafruit_INA219();
    this->_ina216->begin();
}

void Shunt::loop(unsigned long millis) {

}

const char* Shunt::name() {
    return "Shunt";
}

String Shunt::metric() {
    return String("shunt value=" + String(this->_ina216->getPower_mW(), 2));
}