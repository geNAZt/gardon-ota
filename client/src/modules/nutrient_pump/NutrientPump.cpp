//
// Created by FFass on 06/09/2021.
//

#include "NutrientPump.h"
#include "../../util/Debug.h"
#include <Arduino.h>

NutrientPump::NutrientPump(char pin, TDS* tds, CirculationPump* circulationPump) {
    this->_pin = pin;
    this->_tds = tds;
    this->_circulationPump = circulationPump;

    pinMode(this->_pin, OUTPUT);
}

void NutrientPump::loop(unsigned long millis) {
    if (this->_pumpUntil != 0 && millis >= this->_pumpUntil) {
        this->off();
    }

    // We don't pump during circulation
    if (this->_circulationPump->isOn()) {
        return;
    }

    if (this->_inProgressUntil != 0 && millis >= this->_inProgressUntil) {
        this->_inProgressUntil = 0;
    }

    if (this->_inProgressUntil == 0) {
        float ppmValue = this->_tds->ppmValue();
        
        // First check if we need to down pH
        if ( ppmValue > 10 && ppmValue < 950 ) {
            this->_occurence++;
            if (this->_occurence == 5) {
                debug(String("Pumping nutrients..."));
                this->on();
            }
        } else {
            this->_occurence = 0;
        }
    }
}

void NutrientPump::off() {
    digitalWrite(this->_pin, LOW);
    this->_pumpUntil = 0;
    this->_on = false;
}

void NutrientPump::on() {
    digitalWrite(this->_pin, HIGH);
    this->_pumpUntil = millis() + (5 * 200);
    this->_inProgressUntil = millis() + 30 * 1000;
    this->_circulationPump->onFor(10 * 1000);
    this->_on = true;
}

bool NutrientPump::isOn() {
    return this->_on;
}

const char* NutrientPump::name() {
  return "NutrientPump";
}

String NutrientPump::metric() {
    return String("nutrientpump value=" + String((this->isOn()) ? "1" : "0"));
}
