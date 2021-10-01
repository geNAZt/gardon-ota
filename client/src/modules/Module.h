//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_MODULE_H
#define GARDON_MODULE_H

#include <Arduino.h>

class Module {
public:
    virtual void loop(unsigned long millis) = 0;
    virtual const char* name() = 0;
    virtual String metric() = 0;
};


#endif //GARDON_MODULE_H
