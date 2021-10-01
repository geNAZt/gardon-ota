//
// Created by FFass on 06/09/2021.
//

#include "Lamp.h"
#include "../../util/Debug.h"
#include <Arduino.h>

Lamp::Lamp(char pin, NTPClient* timeClient) {
    this->_pin = pin;
    this->_time = timeClient;

    pinMode(this->_pin, OUTPUT);
    this->off();
}

void Lamp::loop(unsigned long millis) {
    if (this->_time->getHours() >= 8 && this->_time->getHours() <= 18) {
        this->on();
    } else {
        this->off();
    }
}

void Lamp::on() {
    this->_on = true;
    digitalWrite(this->_pin, HIGH);
}

void Lamp::off() {
    this->_on = false;
    digitalWrite(this->_pin, LOW);
}

bool Lamp::isOn() {
    return this->_on;
}

const char* Lamp::name() {
  return "Lamp";
}

String Lamp::metric() {
    return String("lamp value=" + String((this->isOn()) ? "1" : "0"));
}
