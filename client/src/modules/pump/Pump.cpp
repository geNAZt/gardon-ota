//
// Created by FFass on 06/09/2021.
//

#include "Pump.h"
#include <Arduino.h>


Pump::Pump(char pin, NTPClient* timeClient) {
    this->_pin = pin;
    this->_time = timeClient;

    pinMode(this->_pin, OUTPUT);
    this->off();
}

void Pump::loop(unsigned long millis) {
    if (this->_onUntil != 0 && millis >= this->_onUntil) {
        this->off();
    }
}

void Pump::off() {
    digitalWrite(this->_pin, LOW);
    this->_on = false;
}

void Pump::onFor(unsigned long forMillis) {
    digitalWrite(this->_pin, HIGH);
    this->_on = true;
    this->_onUntil = millis() + forMillis;
}

bool Pump::isOn() {
    return this->_on;
}

const char* Pump::name() {
  return "Pump";
}

String Pump::metric() {
    return String("pump value=" + String((this->isOn()) ? "1" : "0"));
}