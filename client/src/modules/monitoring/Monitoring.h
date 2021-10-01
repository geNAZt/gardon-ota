//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_MONITORING_H
#define GARDON_MONITORING_H

#include <WiFi.h>

#include "../Module.h"

class Monitoring : public Module {
public:
    explicit Monitoring(Module** modules, unsigned char moduleAmount);

    void loop(unsigned long millis);
    const char* name();
    String metric();

private:
    WiFiUDP _udp;
    Module** _modules;
    unsigned char _moduleAmount;

    unsigned char _index{};
};


#endif //GARDON_MONITORING_H
