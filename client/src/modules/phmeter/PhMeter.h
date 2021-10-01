//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_PHMETER_H
#define GARDON_PHMETER_H

#include "../../util/Average.h"
#include "../Module.h"
#include "../temperature/Temperature.h"

class PhMeter : public Average, public Module {
public:
    explicit PhMeter(char pin, Temperature* temperature);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    float phValue();

    void onFullAverage( unsigned short int );

private:
    Temperature* _temperature;

    float _phValue{};
};


#endif //GARDON_PHMETER_H
