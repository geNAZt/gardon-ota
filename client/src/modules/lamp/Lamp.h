//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_LAMP_H
#define GARDON_LAMP_H

#include <NTPClient.h>
#include "../Module.h"

class Lamp : public Module {
public:
    explicit Lamp(char pin, NTPClient* time);

    void loop(unsigned long millis);
    const char* name();
    String metric();

    bool isOn();

private:
    char _pin;
    NTPClient* _time;

    bool _on{};

    void off();
    void on();
};


#endif //GARDON_LAMP_H
