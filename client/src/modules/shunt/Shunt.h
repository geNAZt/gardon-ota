//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_SHUNT_H
#define GARDON_SHUNT_H

#include <Adafruit_INA219.h>

#include "../Module.h"

class Shunt : public Module {
public:
    explicit Shunt();

    void loop(unsigned long millis);
    const char* name();
    String metric();

private:
    float _milliWatt;
    Adafruit_INA219* _ina216;
};


#endif //GARDON_SHUNT_H
