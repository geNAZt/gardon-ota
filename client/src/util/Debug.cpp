#include "Debug.h"

void debug(String message)  {
    if (Serial.availableForWrite() >= message.length()) 
        Serial.println(message);
}