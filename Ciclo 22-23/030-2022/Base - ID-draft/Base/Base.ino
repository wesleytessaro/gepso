#include "heltec.h"
#include <WiFi.h>
#include "ThingSpeak.h"


#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

const char* ssid = "MEGuest_4178";
const char* password = "12345678";
WiFiClient  client;

unsigned long myChannelNumber = 1548634;
const char * myWriteAPIKey = "9IWG1KSKXYAPLZG9";

void setup() {
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  WiFi.mode(WIFI_STA);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(5000);
    }
    Serial.println("\nConnected.");
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  }
  ThingSpeak.begin(client);  // Initialize ThingSpeak

}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    String req = "";
    while (LoRa.available()) {
      req += (char)LoRa.read();
      //Serial.print((char)LoRa.read());
      //req+=(char)LoRa.read();
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.println("Valor:" + req);
    String temperatura = req.substring(req.indexOf("-") + 1, req.indexOf(":"));
    String condutividade = req.substring(req.indexOf(":") + 1, req.indexOf("+"));
    String id = req.substring(req.indexOf("+") + 1);

    Serial.println(temperatura);
    Serial.println(condutividade);
    Serial.println(id);
    ThingSpeak.setField(1, temperatura);
    ThingSpeak.setField(2, condutividade);
    ThingSpeak.setField(3, id);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
  delay(1);
}
