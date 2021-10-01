#include "API.h"
#include <Arduino.h>

API::API(Moisture* moisture, Pump* pump) {
  this->_server = new WiFiServer(65214);
  this->_server->begin();

  this->_moisture = moisture;
  this->_pump = pump;
}

void API::loop(unsigned long millis) {
    WiFiClient client = this->_server->available();
    if (client) {
        String command = client.readStringUntil('\n');
        if (command == "moist") {
            char buf[12];
            client.write(itoa(this->_moisture->value(), buf, 10));
            client.write('\n');
        } else if (command == "pump") {
            this->_pump->onFor(5000);
            client.write("OK");
            client.write('\n');
        } else if (command == "quit") {
            client.stop();
        }
    }
}

const char* API::name() {
  return "API";
}

String API::metric() {
  return String("");
}