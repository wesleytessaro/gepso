/**
 * @Authors: 
 *  José
 *  Prof. Wagner Moreira
 *  Prof. Wesley Tessaro Andrade
 */

#include <WiFiUdp.h>//Biblioteca do UDP.
WiFiUDP udp;//Cria um objeto da classe UDP.
#include <WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
const int buzzer = 26;   

const char* ssid = "MEGuest_h4178";   // your network SSID (name) 
const char* password = "12345678";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1608236;
const char * myWriteAPIKey = "CK92R06MWQLS0G93";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;


float temperaturaSolo,umidadeSolo, condutividadeSolo;
float temperaturaAmbiente, umidadeAmbiente, pressao, luminosidade;

Adafruit_BME280 bme; //BME280 connect to ESP32 I2C (GPIO 21 = SDA, GPIO 22 = SCL)

void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void setup() {
  Serial.begin(115200);  //Initialize serial
  initBME();
  sensors.begin();
  pinMode(buzzer,OUTPUT);

  WiFi.mode(WIFI_AP);//Define o ESP8266 como Acess Point.
   WiFi.softAP("Rede-Jose", "");//Cria um WiFi de nome "Res-WiFi" e sem senha.
   if(!WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0))){
      Serial.println("AP Config Failed");
   }
   if(WiFi.softAP("Rede-Jose", "")){
      Serial.println("");
      IPAddress myIP = WiFi.softAPIP();
      Serial.println("Network " + String("Rede-Jose") + " running");
      Serial.print("AP IP address: ");
      Serial.println(myIP);
  } else {
      Serial.println("Starting AP failed.");
  }
  udp.begin(5001);//Inicializa a recepçao de dados UDP na porta 5001
  
}

void loop() {

digitalWrite(buzzer, HIGH);
listen();

 
   
}

void listen()//Sub-rotina que verifica se há pacotes UDP's para serem lidos.
{
  String req;
   if (udp.parsePacket() > 0)//Se houver pacotes para serem lidos
   {
       req = "";//Reseta a string para receber uma nova informaçao
       while (udp.available() > 0)//Enquanto houver dados para serem lidos
       {
           char z = udp.read();//Adiciona o byte lido em uma char
           req += z;//Adiciona o char à string
       }
    temperaturaAmbiente = bme.readTemperature();
    umidadeAmbiente = bme.readHumidity();
    pressao = bme.readPressure() / 100.0F;
    luminosidade=analogRead(33);
    sensors.requestTemperatures(); 
    temperaturaSolo = sensors.getTempCByIndex(0);
    umidadeSolo=map(analogRead(34),0,4096,4096,0);
    condutividadeSolo=analogRead(35);

Serial.println(temperaturaAmbiente);
    Serial.println(umidadeAmbiente);
    Serial.println(pressao);
    Serial.println(luminosidade);
    Serial.println(temperaturaSolo);
    Serial.println(umidadeSolo);
    Serial.println(condutividadeSolo);
    Serial.println("------------------");

  
Serial.print("\n ---- \n IP:");
Serial.print(udp.remoteIP());


          udp.beginPacket(udp.remoteIP(), udp.remotePort());
          udp.print("T:");
          udp.print(temperaturaAmbiente);
          udp.print(" / ");
          udp.print("U:");
          udp.print(umidadeAmbiente);
          udp.print(" / ");
          udp.print("P:");
          udp.print(pressao);
          udp.print(" / ");
          udp.print("L:");
          udp.print(luminosidade);
          udp.print(" / ");
          udp.print("TS:");
          udp.print(temperaturaSolo);
          udp.print(" / ");
          udp.print("US:");
          udp.print(umidadeSolo);
          udp.print(" / ");
          udp.print("C:");
          udp.print(condutividadeSolo);
          udp.endPacket();//Finaliza o pacote e envia.


  }
  }
