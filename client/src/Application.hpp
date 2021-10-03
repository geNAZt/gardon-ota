#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <OneWire.h>

#include "modules/phmeter/PhMeter.hpp"
#include "modules/pump/Pump.hpp"
#include "modules/moisture/Moisture.hpp"
#include "modules/lamp/Lamp.hpp"
#include "modules/monitoring/Monitoring.hpp"
#include "modules/phUpDown/PhUpDown.hpp"
#include "modules/tds/TDS.hpp"
#include "modules/nutrient_pump/NutrientPump.hpp"
#include "modules/shunt/Shunt.hpp"
#include "modules/circulation_pump/CirculationPump.hpp"
#include "modules/temperature/Temperature.hpp"
#include "modules/airstone/AirStone.hpp"

#include "networking/ConnectionHandler.h"

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

Module::ModuleBase* modules[11];
Module::Monitoring::Monitoring* monitoring;

NTPClient* timeClient;
WiFiUDP ntpUDP;
OneWire oneWire(ONEWIRE_BUS);
Logger::Logger logger("app");

class Application {
    public:
        Application(Network::ConnectionHandler* connectionHandler) {
            this->_connectionHandler = connectionHandler;
        }

        void setup() {
            // Create a time provider
            timeClient = new NTPClient(ntpUDP, "europe.pool.ntp.org", 2 * 3600, 2 * 60000);

            // Init all modules
            modules[8] = new Module::Shunt::Shunt();
            modules[3] = new Module::Lamp::Lamp(LAMP_PIN, timeClient);
            modules[1] = new Module::Pump::Pump(PUMP_PIN, timeClient);
            modules[4] = new Module::AirStone::AirStone(AIRSTONE_PIN, timeClient);
            modules[9] = new Module::CirculationPump::CirculationPump(CIRCULATION_PUMP_PIN, timeClient);
            modules[10] = new Module::Temperature::Temperature(&oneWire);

            modules[2] = new Module::Moisture::Moisture(MOISTURE_PIN, (Module::Pump::Pump*) modules[1]);
            modules[0] = new Module::PhMeter::PhMeter(PH_PIN, (Module::Temperature::Temperature*) modules[10]);

            modules[6] = new Module::TDS::TDS(TDS_PIN, (Module::Temperature::Temperature*) modules[10]);
            modules[5] = new Module::PhUpDown::PhUpDown(UP_PIN, DOWN_PIN, (Module::PhMeter::PhMeter*) modules[0], (Module::CirculationPump::CirculationPump*) modules[9], (Module::TDS::TDS*) modules[6]);
            modules[7] = new Module::NutrientPump::NutrientPump(NUTRIENT_PUMP_PIN, (Module::TDS::TDS*) modules[6], (Module::CirculationPump::CirculationPump*) modules[9]);

            monitoring = new Module::Monitoring::Monitoring(modules, 11);
        }

        void loop() {
            unsigned long currentMillis = millis();
            unsigned long start = micros();

            timeClient->update();

            // Sensor readings can now happen here
            for (Module::ModuleBase* module : modules) {
                module->loop(currentMillis);
            }

            // Tell monitoring what we did
            monitoring->loop(currentMillis);

            long pause = 10000 - (micros() - start);
            if (pause > 0) {
                delayMicroseconds(pause);
            }
        }

    private:
        Network::ConnectionHandler* _connectionHandler;
};
