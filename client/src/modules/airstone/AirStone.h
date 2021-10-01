//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_AIRSTONE_H
#define GARDON_AIRSTONE_H

#include <NTPClient.h>

#include "../Module.h"

class AirStone : public Module {
public:
    explicit AirStone(char pin, NTPClient* timeClient);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    void onFor(unsigned long forMillis);
    void off();

    bool isOn();

private:
    char _pin;
    
    unsigned long _onUntil{};
    NTPClient* _time;

    bool _on{};
};


#endif //GARDON_AIRSTONE_H
