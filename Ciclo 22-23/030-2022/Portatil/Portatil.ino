#include "heltec.h"
#define BAND    868E6
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 13;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
void setup()
{
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_24);
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "GEPSO");
  Heltec.display->display();
  delay(1000);
  sensors.begin();
  Serial.begin(9600);
}
void loop()
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);


  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_24);

  Heltec.display->drawString(10, 0, String(analogRead(36)));
  Heltec.display->drawString(10, 20, String(temperatureC));
  Heltec.display->display();


  delay(500);//Verificar tempo
}
