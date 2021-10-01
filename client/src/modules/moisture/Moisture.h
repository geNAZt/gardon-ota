//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_MOISTURE_H
#define GARDON_MOISTURE_H

#include "../pump/Pump.h"
#include "../../util/Average.h"
#include "../Module.h"

class Moisture : public Average, public Module {
public:
    explicit Moisture(char pin, Pump* pump);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    float value();

    void onFullAverage( unsigned short int );

private:
    char _pin;
    float _value;
    Pump* _pump;

    float _wantedValue;
};


#endif //GARDON_MOISTURE_H
