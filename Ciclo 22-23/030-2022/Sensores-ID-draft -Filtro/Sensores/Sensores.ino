#include "heltec.h"
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 13;
const int ID = 20;//ID?
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;
//Filtro
unsigned int vout = 0;
unsigned int voutl = 0;
void setup()
{
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
  pinMode(25, OUTPUT);
  sensors.begin();
  Serial.begin(9600);

  if (ID == 20) {
    delay(120000);
  }
}
void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(analogRead(36)));
  Heltec.display->display();
  LoRa.beginPacket();
  LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  // Serial.println(temperatureC);
  analogWrite(25, 256);
  delay(500);
  vout = (vin + 11 * voutl) / 12;
  voutl = vout;
  LoRa.print("-" + String(voutl) + ":");
  LoRa.print(analogRead(36));
  LoRa.print("+" + String(ID));
  LoRa.endPacket();
  analogWrite(25, 0);
  delay(300000);//Verificar tempo
}
