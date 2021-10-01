//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_TDS_H
#define GARDON_TDS_H

#include "../temperature/Temperature.h"
#include "../../util/Average.h"
#include "../Module.h"

class TDS : public Average, public Module {
public:
    explicit TDS(char pin, Temperature* temperature);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    float ppmValue();

    void onFullAverage( unsigned short int );

private:
    Temperature* _temperature;

    float _ppmValue{};
};


#endif //GARDON_TDS_H
