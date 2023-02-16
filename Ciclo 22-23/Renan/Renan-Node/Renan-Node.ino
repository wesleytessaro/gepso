#include <ESP8266WiFi.h>  //Biblioteca do WiFi.
#include <WiFiUdp.h>      //Biblioteca do UDP.
WiFiUDP udp;  //Cria um objeto da classe UDP.

int buzzerPin = D8;
int solenoidePin = D4;


String req = "";
void setup() {

  Serial.begin(9600);
pinMode(buzzerPin,OUTPUT);
pinMode(solenoidePin,OUTPUT);

  WiFi.mode(WIFI_AP);                      //Define o ESP8266 como Acess Point.
  WiFi.softAP("Pressao-GEPSO", "");  //Cria um WiFi de nome "Res-WiFi" e sem senha.
  if (!WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0))) {
    Serial.println("AP Config Failed");
  }

  if (WiFi.softAP("Pressao-GEPSO", "")) {
    Serial.println("");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("Network " + String("Pressao-GEPSO") + " running");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  } else {
    Serial.println("Starting AP failed.");
  }
  delay(2000);      //Aguarda 2 segundos para completar a criaçao do wifi.
  udp.begin(5001);  //Inicializa a recepçao de dados UDP na porta 5001
  
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
    if (req == "h") {
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.println(analogRead(A0));
      udp.endPacket();  //Finaliza o pacote e envia.
    }
    else if (req == "p") {
      //fechar solenoide
      tone(buzzerPin,1500,1000);
      digitalWrite(solenoidePin,LOW);
    }
    else if (req == "i") {
      //abrir solenoide
      tone(buzzerPin,800,1000);
      digitalWrite(solenoidePin,HIGH);
    }
    
  }
}
