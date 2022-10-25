/**
   Sistema para análise de umidade (Capsula)
*/
#include <WiFi.h>
#include "ThingSpeak.h"

WiFiClient  client;

unsigned long myChannelNumber = 1672248;
const char * myWriteAPIKey = "69E1EICCWBGEAZSJ";

// --- Dados de Acesso do seu roteador ---
#define WLAN_SSID   "kareka"   // nome da sua rede
#define WLAN_PASS  "kareka6144"   // senha de acesso do seu roteador


// Variáveis de Tempo
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
void setup() {
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(2, OUTPUT);
  WiFi.begin(WLAN_SSID, WLAN_PASS); //Inicia WiFi com os dados preenchidos
  delay(1000); // aguarda 1 segundo
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) // Tentando conectar na Rede WiFi
  {
    WiFi.begin(WLAN_SSID, WLAN_PASS); //Inicia WiFi com os dados preenchidos
  
    delay(500);
    Serial.print(".");
  }
  Serial.println();

}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    int condutividadeFolha = analogRead(36);
    int umidadeTerra = analogRead(39);
    Serial.println(condutividadeFolha);
    Serial.println(umidadeTerra);
    
    ThingSpeak.setField(1, String(condutividadeFolha)); // Campo 1
     ThingSpeak.setField(2, String(umidadeTerra)); // Campo 1
    
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Canal atualizado com sucesso!");
      digitalWrite(2, HIGH);
      delay(1000);
      digitalWrite(2, LOW);
    }
    else {
      Serial.println("Erro na atualizacao... Erro HTTP:" + String(x));
    }
    lastTime = millis();
  }

}
