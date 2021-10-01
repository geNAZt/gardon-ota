//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_NUTRIENTPUMP_H
#define GARDON_NUTRIENTPUMP_H

#include "../tds/TDS.h"
#include "../circulation_pump/CirculationPump.h"
#include "../Module.h"

class NutrientPump : public Module {
public:
    explicit NutrientPump(char pin, TDS* tds, CirculationPump* circulationPump);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    bool isOn();

private:
    TDS* _tds;
    CirculationPump* _circulationPump;

    unsigned long _inProgressUntil{};
    unsigned long _pumpUntil{};

    char _pin, _occurence{};
    bool _on{};

    void off();
    void on();
};


#endif //GARDON_NUTRIENTPUMP_H
