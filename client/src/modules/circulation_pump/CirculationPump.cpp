//
// Created by FFass on 06/09/2021.
//

#include "CirculationPump.h"
#include <Arduino.h>


CirculationPump::CirculationPump(char pin, NTPClient* timeClient) {
    this->_pin = pin;
    this->_time = timeClient;

    pinMode(this->_pin, OUTPUT);
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
    digitalWrite(this->_pin, LOW);
}

void CirculationPump::onFor(unsigned long forMillis) {
    this->_on = true;
    digitalWrite(this->_pin, HIGH);
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