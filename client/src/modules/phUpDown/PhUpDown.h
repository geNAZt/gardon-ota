//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_PHUPDOWN_H
#define GARDON_PHUPDOWN_H

#include "../phmeter/PhMeter.h"
#include "../circulation_pump/CirculationPump.h"
#include "../Module.h"

class PhUpDown : public Module {
public:
    explicit PhUpDown(char upPin, char downPin, PhMeter* phMeter, CirculationPump* circulationPump);

    void loop(unsigned long millis);
    const char* name();
    String metric();

private:
    PhMeter* _phMeter;
    CirculationPump* _circulationPump;

    float _downReachTarget{}, _upReachTarget{};

    unsigned long _inProgressUntil{};
    unsigned long _pumpUntil{};

    char _upPin;
    char _downPin;


    char _direction{}, _occurence{};

    void allOff();
    void down();
    void up();
};


#endif //GARDON_PHUPDOWN_H
