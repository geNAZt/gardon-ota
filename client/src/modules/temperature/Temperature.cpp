#include "Temperature.h"
#include "../../util/Debug.h"
#include <Arduino.h>

Temperature::Temperature(OneWire* oneWire) {
    this->_temperatureSensor = new DallasTemperature(oneWire);
    this->_temperatureSensor->begin();
    this->_temperatureSensor->setWaitForConversion(false);
}

void Temperature::loop(unsigned long millis) {
    if (this->_temperatureSensor->isConversionComplete())
        this->_temperatureSensor->requestTemperatures();
}

float Temperature::temperature() {
    return this->_temperatureSensor->getTempCByIndex(0);
}

const char* Temperature::name() {
  return "Temperature";
}

String Temperature::metric() {
    return String("temperature value=" + String(this->temperature(), 3));
}
