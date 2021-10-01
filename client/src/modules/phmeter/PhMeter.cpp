//
// Created by FFass on 06/09/2021.
//

#include "PhMeter.h"
#include "../../util/Debug.h"
#include <Arduino.h>

#define ADC_MAX 4096
#define PH_PER_C 0.02f

float VOLT_FOR_4_PH = 1.93359375f;
float VOLT_FOR_7_PH = 1.421997f;

float m = 3 / (VOLT_FOR_7_PH - VOLT_FOR_4_PH);
float b = 4 - (VOLT_FOR_4_PH * m);

PhMeter::PhMeter(char pin, Temperature* temperature) : Average::Average(pin) {
    this->_temperature = temperature;
}

void PhMeter::onFullAverage(unsigned short int average) {
    float voltage = ((float) average * 3.3f) / ADC_MAX;
    float tempPhValue = m * voltage + b;
    
    float tempC = this->_temperature->temperature();
    float diffC = tempC - 25.0f;

    this->_phValue = tempPhValue + (diffC * PH_PER_C);
}

///
/// This method uses 112 micros on read, 260 micros per 10 runs on loop
///
void PhMeter::loop(unsigned long millis) {
    Average::loop();
}

float PhMeter::phValue() {
    return this->_phValue;
}

const char* PhMeter::name() {
  return "PhMeter";
}

String PhMeter::metric() {
    return String("ph value=" + String(this->phValue(), 3));
}
