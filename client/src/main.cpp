#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Update.h>

#include "esp_ota_ops.h"
#include "esp_image_format.h"

#include "FS.h"
#include "SPIFFS.h"

#include "networking/ConnectionHandler.h"
#include "networking/packet/CurrentVersionPacket.h"

WiFiClientSecure connection;
Network::ConnectionHandler connectionHandler(&connection);

void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

void setup() {
  Serial.begin(9600);

  // Mount internal file system
  if(!SPIFFS.begin(false)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Check if there is a WIFI configuration, if not we die
  File wifiConfig = SPIFFS.open("/wifi.conf.json");
  if (!wifiConfig) {
    Serial.println("No wifi config found");
    return;
  }

  // Parse wifi config
  const size_t capacity = JSON_OBJECT_SIZE(2) + 40 + 40;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, wifiConfig);
  if (error.code() != DeserializationError::Code::Ok) {
    Serial.print(error.c_str());
    Serial.println("Can't parse json");
  }

  wifiConfig.close();

  // Connect to wifi
  const char* ssid = doc["ssid"];
  const char* password = doc["password"];

  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  Serial.println("Connected to wifi...");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Read all certs we need
  File ca = SPIFFS.open("/ca.crt");
  connection.loadCACert(ca, ca.size());
  ca.close();

  File clientCert = SPIFFS.open("/client.crt");
  connection.loadCertificate(clientCert, clientCert.size());
  clientCert.close();

  File clientKey = SPIFFS.open("/client.key");
  connection.loadPrivateKey(clientKey, clientKey.size());
  clientKey.close();

  // Connecting to control
  connection.connect((const char*) doc["remote"], 65432, 5000);

  // Check what firmware we currently run
  String hash = ESP.getSketchMD5();
  Serial.print("Current firmware hash ");
  Serial.println(hash);

  // Tell the server which firmware we have so it can decide if we need a new one
  Network::Packet::CurrentVersionPacket* packet = new Network::Packet::CurrentVersionPacket();
  packet->firmwareChecksum(hash);
  connectionHandler.write(packet);
}

void loop() {
  connectionHandler.loop();

  /*if (connection.available() > 0) {
    uint8_t* packetSizeBytes = (uint8_t*) malloc(2);
    connection.readBytes(packetSizeBytes, 2);

    unsigned short packetSize = (packetSizeBytes[0] << 8) | packetSizeBytes[1];
    free(packetSizeBytes);
    Serial.print("Got ");
    Serial.print(packetSize);
    Serial.print(" bytes");

    uint8_t* data = (uint8_t*) malloc(packetSize);
    connection.readBytes(data, packetSize);
    free(data);
  }

  // put your main code here, to run repeatedly:
  size_t packetSize = 2 + 1 + 2 + 5;
  uint8_t* packet = (uint8_t*) malloc(packetSize);
  packet[0] = 0;
  packet[1] = 8;
  packet[2] = 1;
  packet[3] = 0;
  packet[4] = 5;
  packet[5] = 'H';
  packet[6] = 'e';
  packet[7] = 'l';
  packet[8] = 'l';
  packet[9] = 'o';
  connection.write((const uint8_t*) packet, packetSize);

  free(packet);

  // We are connected, free read file
  Serial.println(ESP.getFreeHeap());*/
}