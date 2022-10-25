
#include "heltec.h"
#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;


/*#include <SPI.h>
  #define BME_SCK 18
  #define BME_MISO 19
  #define BME_MOSI 23
  #define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)
WiFiClient  client;
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;
const char* ssid = "Ricardo";   // your network SSID (name)
const char* password = "12345678";   // your network password

unsigned long myChannelNumber = 1608236;
const char * myWriteAPIKey = "CK92R06MWQLS0G93";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

float temperatureC;
float humidity;
float pressure;
int humiditySoil;
int bombStatus = 0;
int limiarUmidade = 1000;
void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  WiFi.mode(WIFI_STA);
  // Connect or reconnect to WiFi
  
  ThingSpeak.begin(client);

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}


void loop() {
  if ((millis() - lastTime) > timerDelay) {

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      int tempoConectar = millis();
      Serial.print("Attempting to connect");
      while ((WiFi.status() != WL_CONNECTED) && (millis() - tempoConectar) < 10000) {
        Serial.print(".");
        WiFi.begin(ssid, password);
        delay(10);
      }
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nErro!!!");
      }
      else{
        Serial.println("\nConectado!!!");
      }
    }

    printValues();



  }



  enviarBomba();
  delay(delayTime);
}

void printValues() {
  bme.begin(0x76);
  temperatureC = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humiditySoil = map(analogRead(36), 0, 4095, 4095, 0);
  Serial.print("Temperature = ");
  Serial.print(temperatureC);
  Serial.println(" *C");

  Serial.print("humidity = ");
  Serial.print(humidity);
  Serial.println("  %");

  Serial.print("pressure = ");
  Serial.print(pressure);
  Serial.println("  kpa");

  Serial.print("Soil= ");
  Serial.print(humiditySoil);
  Serial.println("  ");


  ThingSpeak.setField(1, temperatureC);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, pressure);
  ThingSpeak.setField(4, humiditySoil);
  ThingSpeak.setField(5, bombStatus);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  lastTime = millis();
}
void enviarBomba() {
  // send packet
  if (humiditySoil < limiarUmidade) {
    humiditySoil = map(analogRead(36), 0, 4095, 4095, 0);

    if (humiditySoil < limiarUmidade) {

      bombStatus = 1;
      LoRa.beginPacket();
      LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print(bombStatus);
      LoRa.endPacket();
    }

  }
  else {
    bombStatus = 0;
    LoRa.beginPacket();
    LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print(bombStatus);
    LoRa.endPacket();
  }


}
