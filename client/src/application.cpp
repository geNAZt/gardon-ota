#include "application.h"

#include <Arduino.h>
#include "modules/phmeter/PhMeter.h"
#include "modules/pump/Pump.h"
#include "modules/api/Api.h"
#include "modules/moisture/Moisture.h"
#include "modules/lamp/Lamp.h"
#include "modules/monitoring/Monitoring.h"
#include "modules/phUpDown/PhUpDown.h"
#include "modules/tds/TDS.h"
#include "modules/nutrient_pump/NutrientPump.h"
#include "modules/shunt/Shunt.h"
#include "modules/circulation_pump/CirculationPump.h"
#include "modules/temperature/Temperature.h"
#include "modules/airstone/AirStone.h"

#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <OneWire.h>

#define ONEWIRE_BUS 4
#define PH_PIN 36   
#define TDS_PIN 34     
#define PUMP_PIN 33 
#define LAMP_PIN 32
#define NUTRIENT_PUMP_PIN 5
#define MOISTURE_PIN 39
#define UP_PIN 16
#define DOWN_PIN 17
#define CIRCULATION_PUMP_PIN 25
#define AIRSTONE_PIN 26

Module* modules[12];
Monitoring* monitoring;
NTPClient* timeClient;
WiFiUDP ntpUDP;
OneWire oneWire(ONEWIRE_BUS);

Application::Application(Network::ConnectionHandler* connectionHandler) {
    this->_connectionHandler = connectionHandler;
}

void Application::setup() {
    // Create a time provider
    timeClient = new NTPClient(ntpUDP, "europe.pool.ntp.org", 2 * 3600, 60000);

    // Init all modules
    modules[8] = new Shunt();
    modules[3] = new Lamp(LAMP_PIN, timeClient);
    modules[1] = new Pump(PUMP_PIN, timeClient);
    modules[11] = new AirStone(AIRSTONE_PIN, timeClient);
    modules[9] = new CirculationPump(CIRCULATION_PUMP_PIN, timeClient);
    modules[10] = new Temperature(&oneWire);

    modules[2] = new Moisture(MOISTURE_PIN, (Pump*) modules[1]);
    modules[4] = new API((Moisture*) modules[2], (Pump*) modules[1]);

    modules[0] = new PhMeter(PH_PIN, (Temperature*) modules[10]);
    modules[5] = new PhUpDown(UP_PIN, DOWN_PIN, (PhMeter*) modules[0], (CirculationPump*) modules[9]);

    modules[6] = new TDS(TDS_PIN, (Temperature*) modules[10]);
    modules[7] = new NutrientPump(NUTRIENT_PUMP_PIN, (TDS*) modules[6], (CirculationPump*) modules[9]);

    monitoring = new Monitoring(modules, 12);
}

void Application::loop() {
    unsigned long currentMillis = millis();
    unsigned long start = micros();

    timeClient->update();

    // Sensor readings can now happen here
    for (Module* module : modules) {
        module->loop(currentMillis);
    }

    // Tell monitoring what we did
    monitoring->loop(currentMillis);

    long pause = 10000 - (micros() - start);
    if (pause > 0) {
        delayMicroseconds(pause);
    }
}