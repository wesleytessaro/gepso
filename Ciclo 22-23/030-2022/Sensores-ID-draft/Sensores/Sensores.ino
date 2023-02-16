
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

  pinMode(25,OUTPUT);
  sensors.begin();

  Serial.begin(9600);
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(analogRead(36)));
  Heltec.display->display();

  // send packet
  LoRa.beginPacket();
  
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  // Serial.println(temperatureC);
   
  analogWrite(25,256);
  delay(500);
  LoRa.print("-"+String(temperatureC)+":");
  //LoRa.print("-123:");
  LoRa.print(analogRead(36));
  LoRa.print("+"+String(ID));
 // Serial.println(analogRead(36));
    //Serial.println("----------");
  LoRa.endPacket();
  analogWrite(25,0);
  
  
  delay(60000);//Verificar tempo  
  
  
}
