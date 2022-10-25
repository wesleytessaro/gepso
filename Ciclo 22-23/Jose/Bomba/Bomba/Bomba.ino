/* 
  Check the new incoming messages, and print via serialin 115200 baud rate.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define PINO_BOMBA 12

int ultimaChegada=0;
void setup() {
    //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  pinMode(12,OUTPUT);
   digitalWrite(12,HIGH);  
}

void loop() {

  if ((millis()-ultimaChegada)>15000){
    digitalWrite(12,HIGH);
  }
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      char in = (char)LoRa.read();
      Serial.println(in);
      //Serial.print((char)LoRa.read());
      ultimaChegada=millis();
      if (in=='0'){
        digitalWrite(12,HIGH);  
      }
      else{
         digitalWrite(12,LOW);  
       }
      
      
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
