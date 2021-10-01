//
// Created by FFass on 06/09/2021.
//

#include "TDS.h"
#include "../../util/Debug.h"

#include <Arduino.h>

#define ADC_MAX 4096

TDS::TDS(char pin, Temperature* temperature) : Average::Average(pin) {
    this->_temperature = temperature;
}

void TDS::onFullAverage(unsigned short int average) {
    float voltage = (((float) average) * 3.3f) / ADC_MAX;
    float compensationCoefficient = 1.0 + 0.02 * (this->_temperature->temperature() - 25.0f);
    float compensationVoltage = voltage / compensationCoefficient;

    float tempPPMValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
    this->_ppmValue = tempPPMValue;
}

void TDS::loop(unsigned long millis) {
    Average::loop();
}

float TDS::ppmValue() {
    return this->_ppmValue;
}

const char* TDS::name() {
  return "TDS";
}

String TDS::metric() {
    return String("tds value=" + String(this->ppmValue(), 3));
}
