//
// Created by FFass on 06/09/2021.
//

#include "Average.h"
#include <Arduino.h>

Average::Average(char pin) {
    this->_pin = pin;
}

void Average::loop() {
    this->_buf[this->_index] = analogRead(this->_pin);
    if (this->_index == AVERAGE_SIZE - 1) {
        for (int i = 0; i < AVERAGE_SIZE - 1; i++) {
            for (int j = i + 1; j < AVERAGE_SIZE; j++) {
                if (this->_buf[i] > this->_buf[j]) {
                    this->_temp = this->_buf[i];
                    this->_buf[i] = this->_buf[j];
                    this->_buf[j] = this->_temp;
                }
            }
        }

        this->_avgValue = 0;
        for (int i = 0; i < AVERAGE_SIZE; i++)
            this->_avgValue += this->_buf[i];

        this->onFullAverage(this->_avgValue / AVERAGE_SIZE);

        this->_index = 0;
    } else {
        this->_index++;
    }
}
