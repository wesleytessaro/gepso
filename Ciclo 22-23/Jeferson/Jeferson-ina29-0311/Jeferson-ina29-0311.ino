#include <ESP8266WiFi.h>  //Biblioteca do WiFi.
#include <WiFiUdp.h>      //Biblioteca do UDP.
WiFiUDP udp;              //Cria um objeto da classe UDP.
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;


String req = "";
void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);                      //Define o ESP8266 como Acess Point.
  WiFi.softAP("Condutivimetro-4-CH", "");  //Cria um WiFi de nome "Res-WiFi" e sem senha.
  if (!WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0))) {
    Serial.println("AP Config Failed");
  }

  if (WiFi.softAP("Condutivimetro-INA", "")) {
    Serial.println("");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("Network " + String("Condutivimetro-INA") + " running");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  } else {
    Serial.println("Starting AP failed.");
  }
  uint32_t currentFrequency;

  Serial.println("Hello!");

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");

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
    if (req == "a") {
      String condutividade = lerCondutividade();
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.println(condutividade);
      udp.endPacket();  //Finaliza o pacote e envia.
    }
  }
}

String lerCondutividade() {
  String condutividade = "";
  float shuntvoltage = 0;
  float current_mA = 0;
  float power_mW = 0;

  for (int i = 0; i < 5; i++) {
    shuntvoltage += ina219.getShuntVoltage_mV();
    current_mA += ina219.getCurrent_mA();
    power_mW += ina219.getPower_mW();
    delay(200);
  }
  shuntvoltage = shuntvoltage / 5;
  current_mA = current_mA / 5;
  power_mW = power_mW / 5;

  condutividade = String(shuntvoltage) + " , " + String(current_mA) + " , " + String (power_mW);
  Serial.println(condutividade);
  return condutividade;
}