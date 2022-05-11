/**
 * Código Penetrometro - HTTP
 * 
 */
#include "Adafruit_VL53L0X.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <HX711.h> //adiciona a biblioteca ao codigo

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

HX711 escala; //declaracao do objeto escala na classe HX711 da biblioteca
float fator_calibracao = 46500; //pre-definicao da variavel de calibracao

Adafruit_VL53L0X lox = Adafruit_VL53L0X();


//configuracao dos pinos para o modulo HX711
const int PINO_DT = D5;
const int PINO_SCK = D6;

float resistencia;
unsigned long segundosAnalise = 20; //20 segundos?
const int buzzer =  D8;
const int led = D9;


String analiseConcluida = " ";
boolean analiseSendoFeita = false;
String titulo = "";
/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);     // Initialize GPIO2 pin as an output
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  escala.begin (PINO_DT, PINO_SCK);
  float media_leitura = escala.read_average();
  Serial.print("Media de leituras com Celula sem carga: ");
  Serial.print(media_leitura);
  Serial.println();
  escala.tare(); //zera a escala
  lox.begin();


  WiFi.mode(WIFI_AP);//Define o ESP8266 como Acess Point.

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/analise", handle_analise);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  Serial.println("Conectado");
  titulo = "Equipamento Disponivel";
  server.send(200, "text/html", SendHTML());
}

void handle_analise() {//Fazer análise
  delay(5000);
  handleSensores();
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/html", SendHTML());

}


void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html lang=\"pt-br\" > \n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no, charset=utf-8\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  //ptr += "<h1>Penetrometro - GEPSO</h1>\n";
  ptr += analiseConcluida;
  ptr += "<a class=\"button button-on\" href=\"/analise\">Fazer Analise</a>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
void handleSensores() {
  unsigned long tempoAtual = millis();
  acionaLuz();
  tone(buzzer,800,1000);
  Serial.println("Iniciando análise em 2 seg");
  Serial.println("Analise Iniciada");
  escala.tare(); //zera a escala

  String distancia = "";
  String   resistencia = "";
  String   umidade = "";

  while ((tempoAtual + segundosAnalise * 1000) > millis()) {
    distancia += getDistancia() + ",";
    resistencia += getResistencia() + ",";
    umidade += getUmidade() + ",";
    delay(75);
  }
  distancia.setCharAt(distancia.length() - 1, ' ');
  resistencia.setCharAt(resistencia.length() - 1, ' ');
  umidade.setCharAt(umidade.length() - 1, ' ');
  analiseConcluida = "\"distancia\":[" + distancia + "],\"resistencia\":[" + resistencia + "],\"umidade\":[" + umidade + "]";

  //  dados="distancia:,"+distancia+" \n "+"resistencia:,"+resistencia+" \n "+"umidade:,"+umidade;

  apagaLuz();
  tone(buzzer,1000,1000);
  Serial.println("finalizada:" + analiseConcluida);
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
  return String(RP);
  //return "0";
}
String getUmidade() {
 return String(map(analogRead(A0),0,1024,1024,0));
}


void apagaLuz() {
  digitalWrite(led,LOW);
}

void acionaLuz() {
digitalWrite(led,HIGH);
}
