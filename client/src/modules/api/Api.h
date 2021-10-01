//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_API_H
#define GARDON_API_H

#include "../moisture/Moisture.h"
#include "../pump/Pump.h"
#include "../Module.h"
#include <SPI.h>
#include <WiFi.h>

class API : public Module {
public:
    explicit API(Moisture* moisture, Pump* pump);

    void loop(unsigned long millis);
    const char* name();
    String metric();

private:
    WiFiServer* _server;

    Moisture* _moisture;
    Pump* _pump;
};


#endif //GARDON_API_H
