/**
   Código Penetrometro - HTTP

*/
#include <HX711.h> //adiciona a biblioteca ao codigo
#include "Adafruit_VL53L0X.h"


Adafruit_VL53L0X lox = Adafruit_VL53L0X();


HX711 escala; //declaracao do objeto escala na classe HX711 da biblioteca
float fator_calibracao = 46500; //pre-definicao da variavel de calibracao



//configuracao dos pinos para o modulo HX711
const int PINO_DT = D5;
const int PINO_SCK = D6;

float resistencia;

void setup() {
    lox.begin();

  escala.begin (PINO_DT, PINO_SCK);
  float media_leitura = escala.read_average();
  Serial.print("Media de leituras com Celula sem carga: ");
  Serial.print(media_leitura);
  Serial.println();
  escala.tare(); //zera a escala
Serial.begin(115200);



}
void loop() {
  escala.set_scale(fator_calibracao);
  //float RP=(9.80665/((0.00028)*1000000))*escala.get_units();
  float RP = escala.get_units()*-1;
  Serial.print(RP);
  getDistancia();

  
}

String getDistancia() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4)
  { // phase failures have incorrect data
    Serial.print(",");
     Serial.print(measure.RangeMilliMeter);
     Serial.println();
    return String(measure.RangeMilliMeter);
  }
  else
  {
    Serial.println(" out of range ");
  }
  return String("0");
}
