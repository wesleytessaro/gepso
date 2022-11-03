#include <ESP8266WiFi.h>  //Biblioteca do WiFi.
#include <WiFiUdp.h>      //Biblioteca do UDP.
WiFiUDP udp;              //Cria um objeto da classe UDP.

const int RELE_1 = D3;
const int RELE_2 = D4;
const int RELE_3 = D5;
const int RELE_4 = D6;
const int HASTE = D0;
/**
RELE 1: 510 OHMS
RELE 2: 265.10 OHMS
RELE 3: 328,3 OHMS
RELE 4: 131,2 OHMS
*/
String req = "";
void setup() {
  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
  pinMode(RELE_3, OUTPUT);
  pinMode(RELE_4, OUTPUT);
  pinMode(HASTE, OUTPUT);
  Serial.begin(9600);

  digitalWrite(HASTE, LOW);

  WiFi.mode(WIFI_AP);                      //Define o ESP8266 como Acess Point.
  WiFi.softAP("Condutivimetro-4-CH", "");  //Cria um WiFi de nome "Res-WiFi" e sem senha.
  if (!WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0))) {
    Serial.println("AP Config Failed");
  }

  if (WiFi.softAP("Condutivimetro-4-CH", "")) {
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
}

void loop() {
  listen();  //Sub-rotina para verificar a existencia de pacotes UDP.
}
void selecionaResistor(int rele) {

  if (rele == 1) {
    digitalWrite(RELE_1, LOW);
    digitalWrite(RELE_2, HIGH);
    digitalWrite(RELE_3, HIGH);
    digitalWrite(RELE_4, HIGH);
  } else if (rele == 2) {
    digitalWrite(RELE_1, HIGH);
    digitalWrite(RELE_2, LOW);
    digitalWrite(RELE_3, HIGH);
    digitalWrite(RELE_4, HIGH);
  } else if (rele == 3) {
    digitalWrite(RELE_1, HIGH);
    digitalWrite(RELE_2, HIGH);
    digitalWrite(RELE_3, LOW);
    digitalWrite(RELE_4, HIGH);
  } else {
    digitalWrite(RELE_1, LOW);
    digitalWrite(RELE_2, HIGH);
    digitalWrite(RELE_3, HIGH);
    digitalWrite(RELE_4, LOW);
  }
  delay(200);
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
      String condutividade = lerCondutividade();
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.println(condutividade);
      udp.endPacket();  //Finaliza o pacote e envia.
    }
  }
}

String lerCondutividade() {
  String condutividade="";
  selecionaResistor(1);
  condutividade=condutividade+analogRead(A0);
  selecionaResistor(2);
  condutividade=condutividade+","+analogRead(A0);
  selecionaResistor(3);
  condutividade=condutividade+","+analogRead(A0);
  selecionaResistor(4);
  condutividade=condutividade+","+analogRead(A0);
  
  Serial.println(condutividade);
  return condutividade;
}