//
// Created by FFass on 06/09/2021.
//

#include "AirStone.h"
#include <Arduino.h>


AirStone::AirStone(char pin, NTPClient* timeClient) {
    this->_pin = pin;
    this->_time = timeClient;

    pinMode(this->_pin, OUTPUT);
    this->off();
}

void AirStone::loop(unsigned long millis) {
    if (this->_onUntil != 0 && millis >= this->_onUntil) {
        this->off();
    }

    // Time to start table
    if ((this->_time->getMinutes() == 0 || this->_time->getMinutes() == 30) && this->_time->getSeconds() == 0) {
        this->onFor(5 * 1000);
    }
}

void AirStone::off() {
    digitalWrite(this->_pin, LOW);
    this->_on = false;
}

void AirStone::onFor(unsigned long forMillis) {
    digitalWrite(this->_pin, HIGH);
    this->_on = true;
    this->_onUntil = millis() + forMillis;
}

bool AirStone::isOn() {
    return this->_on;
}

const char* AirStone::name() {
  return "AirStone";
}

String AirStone::metric() {
    return String("airstone value=" + String((this->isOn()) ? "1" : "0"));
}