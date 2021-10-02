//
// Created by FFass on 06/09/2021.
//

#include "CirculationPump.h"
#include <Arduino.h>


CirculationPump::CirculationPump(char pin, NTPClient* timeClient) {
    this->_pin = pin;
    this->_time = timeClient;

    // Setup PWM
    ledcSetup(0, 30000, 8);
    ledcAttachPin(this->_pin, 0);

    this->off();
}

void CirculationPump::loop(unsigned long millis) {
    if (this->_onUntil != 0 && millis >= this->_onUntil) {
        this->off();
    }

    // Time to start table
    if ((this->_time->getMinutes() == 0 || this->_time->getMinutes() == 30) && this->_time->getSeconds() == 0) {
        this->onFor(30 * 1000);
    }
}

void CirculationPump::off() {
    this->_on = false;
    ledcWrite(this->_pin, 0);
}

void CirculationPump::onFor(unsigned long forMillis) {
    this->_on = true;
    ledcWrite(this->_pin, 85);
    this->_onUntil = millis() + forMillis;
}

bool CirculationPump::isOn() {
    return this->_on;
}

const char* CirculationPump::name() {
  return "CirculationPump";
}

String CirculationPump::metric() {
    return String("circulationpump value=" + String((this->isOn()) ? "1" : "0"));
}