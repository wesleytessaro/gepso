/**
   Código Penetrometro - HTTP

*/
#include "Adafruit_VL53L0X.h"
#include <ESP8266WiFi.h>

#include <WiFiUdp.h>      //Biblioteca do UDP.
#include <HX711.h> //adiciona a biblioteca ao codigo

/* Put your SSID & Password */
const char* ssid = "Resistencia";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

HX711 escala; //declaracao do objeto escala na classe HX711 da biblioteca
float fator_calibracao = 46500; //pre-definicao da variavel de calibracao

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

WiFiUDP udp;  //Cria um objeto da classe UDP.

//configuracao dos pinos para o modulo HX711
const int PINO_DT = D5;
const int PINO_SCK = D6;

float resistencia;
unsigned long segundosAnalise = 20; //20 segundos?
const int buzzerPin =  D8;
const int led = D9;


String analiseConcluida = " ";
boolean analiseSendoFeita = false;
String titulo = "";
String req = "";
/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);




void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);     // Initialize GPIO2 pin as an output
  pinMode(led, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  escala.begin (PINO_DT, PINO_SCK);
  float media_leitura = escala.read_average();
  Serial.print("Media de leituras com Celula sem carga: ");
  Serial.print(media_leitura);
  Serial.println();
  escala.tare(); //zera a escala
  lox.begin();

  WiFi.mode(WIFI_AP);                      //Define o ESP8266 como Acess Point.
  WiFi.softAP("Pressao-GEPSO", "");  //Cria um WiFi de nome "Res-WiFi" e sem senha.
  if (!WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0))) {
    Serial.println("AP Config Failed");
  }

  if (WiFi.softAP("Resistencia-GEPSO", "")) {
    Serial.println("");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("Network " + String("Pressao-GEPSO") + " running");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  } else {
    Serial.println("Starting AP failed.");
  }
  delay(2000);      //Aguarda 2 segundos


  delay(100);
  udp.begin(5001);  //Inicializa a recepçao de dados UDP na porta 5001

}
void loop() {
  listen();  //Sub-rotina para verificar a existencia
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
      udp.println(String(getDistancia()) + "," + String(getResistencia()));
      udp.endPacket();  //Finaliza o pacote e envia.
    }
    else if (req == "p") {
      //parar
      tone(buzzerPin, 1500, 1000);
    }
    else if (req == "i") {
      //iniciar
      escala.tare(); //zera a escala
      lox.begin();
      tone(buzzerPin, 800, 1000);
    }

  }
}
String getDistancia() {
  VL53L0X_RangingMeasurementData_t measure;
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4)
  { // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    return String(measure.RangeMilliMeter);
  }
  else
  {
    Serial.println(" out of range ");
  }
  return String("0");
}
String getResistencia() {
  escala.set_scale(fator_calibracao);
  //float RP=(9.80665/((0.00028)*1000000))*escala.get_units();
  float RP = escala.get_units();
  return String(-RP);
  //return "0";
}
String getUmidade() {
  return String(map(analogRead(A0), 0, 1024, 1024, 0));
}


void apagaLuz() {
  digitalWrite(led, LOW);
}

void acionaLuz() {
  digitalWrite(led, HIGH);
}
