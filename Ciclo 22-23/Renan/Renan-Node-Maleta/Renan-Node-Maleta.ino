#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>  //Biblioteca do WiFi.
#include <WiFiUdp.h>      //Biblioteca do UDP.
#include <SPI.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiUDP udp;  //Cria um objeto da classe UDP.

int buzzerPin = D8;
int solenoidePin = D4;
boolean reading = false;

String req = "";
void setup() {


  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(solenoidePin, OUTPUT);
  digitalWrite(solenoidePin, HIGH);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(1000);
  escreverTela("GEPSO", 3, 0, 10, true);
  delay(2000);

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
  if (reading)
    Serial.println(String(millis()) + "," + String(analogRead(A0)));
  else
    escreverTela(String(analogRead(A0)), 6, 0, 10, true);

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
      tone(buzzerPin, 1500, 1000);
      digitalWrite(solenoidePin, HIGH);
      Serial.println("----------------Finalizada-----------------");
      reading = false;
    }
    else if (req == "i") {
      //abrir solenoide
      tone(buzzerPin, 800, 1000);
      reading = true;
      Serial.println("----------------Iniciando-----------------");
      digitalWrite(solenoidePin, LOW);
    }

  }
}

void escreverTela(String texto, int tamanho, int x, int y, boolean apagar) {
  if (apagar)
    display.clearDisplay();
  display.setTextSize(tamanho);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(texto);
  display.display();
}
