#pragma once

#define AVERAGE_SIZE 100

#include <Arduino.h>

class Average {
public:
    explicit Average(char pin) {
        this->_pin = pin;
    }

    void loop() {
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

    virtual void onFullAverage(unsigned short int average) = 0;
private:
    char _pin;
    
    unsigned int _avgValue{};
    unsigned short int _buf[AVERAGE_SIZE]{}, _temp{};

    char _index{};
};
