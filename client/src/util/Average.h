//
// Created by FFass on 06/09/2021.
//

#ifndef GARDON_AVERAGE_H
#define GARDON_AVERAGE_H

#define AVERAGE_SIZE 100

class Average {
public:
    explicit Average(char pin);

    void loop();

    virtual void onFullAverage(unsigned short int average) = 0;
private:
    char _pin;
    
    unsigned int _avgValue{};
    unsigned short int _buf[AVERAGE_SIZE]{}, _temp{};

    char _index{};
};


#endif //GARDON_AVERAGE_H
