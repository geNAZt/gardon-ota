//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_TEMPERATURE_H
#define GARDON_TEMPERATURE_H

#include "../Module.h"

#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature : public Module {
public:
    explicit Temperature(OneWire* oneWire);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    float temperature();

private:
    DallasTemperature* _temperatureSensor;
};


#endif //GARDON_TEMPERATURE_H
