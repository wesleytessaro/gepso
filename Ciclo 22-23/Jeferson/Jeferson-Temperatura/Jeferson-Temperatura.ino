#include <ESP8266WiFi.h>  //Biblioteca do WiFi.
#include <WiFiUdp.h>      //Biblioteca do UDP.
#include <OneWire.h>
#include <DallasTemperature.h>
WiFiUDP udp;  //Cria um objeto da classe UDP.

const int oneWireBus = D2;
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);



String req = "";
void setup() {

  Serial.begin(9600);



  WiFi.mode(WIFI_AP);                      //Define o ESP8266 como Acess Point.
  WiFi.softAP("Condutivimetro-4-CH", "");  //Cria um WiFi de nome "Res-WiFi" e sem senha.
  if (!WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0))) {
    Serial.println("AP Config Failed");
  }

  if (WiFi.softAP("Condutivimetro-TEM", "")) {
    Serial.println("");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("Network " + String("Condutivimetro-4-CH") + " running");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  } else {
    Serial.println("Starting AP failed.");
  }
  delay(2000);      //Aguarda 2 segundos para completar a criaçao do wifi.
  udp.begin(5001);  //Inicializa a recepçao de dados UDP na porta 5001
  sensors.begin();
}

void loop() {
  listen();  //Sub-rotina para verificar a existencia de pacotes UDP.
}


void listen()  //Sub-rotina que verifica se há pacotes UDP's para serem lidos.
{
  if (udp.parsePacket() > 0)  //Se houver pacotes para serem lidos
  {
    req = "";                    //Reseta a string para receber uma nova informaçao
    while (udp.available() > 0)  //Enquanto houver dados para serem lidos
    {
      char z = udp.read();  //Adiciona o byte lido em uma char
      req += z;             //Adiciona o char à string
    }
    if (req == "a") {
      sensors.requestTemperatures();
      float temperatureC = sensors.getTempCByIndex(0);
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.println(temperatureC);
      udp.endPacket();  //Finaliza o pacote e envia.
    }
  }
}

