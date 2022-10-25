#include "HX711.h"
#define DOUT A0
#define CLK  A1
HX711 bau;             // instancia Balança HX711
float calibration_factor = 160340;     // fator de calibração aferido na Calibração

#define PIN_D0 6
#define PIN_D1 7

int estado = 0;

void setup()
{
  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  pinMode(5, INPUT);
  Serial.begin(9600);
  bau.begin(DOUT, CLK);      // inicializa a balança
  bau.set_scale(calibration_factor);             // ajusta fator de calibração
  bau.tare();                                    // zera a Balança
  digitalWrite(PIN_D0, LOW);
  digitalWrite(PIN_D1, LOW);
}

void loop()
{

  if (Serial.available())                            // se a serial estiver disponivel
  {
    char temp = Serial.read();
    if (temp == 'a' || temp == 'A')     {
      estado = 1;
      digitalWrite(PIN_D0, LOW);
      digitalWrite(PIN_D1, LOW);
      Serial.println("------------- Comando para desligamento ---------");
    }
    else if (temp == 'b' || temp == 'B')
    {
      estado = 2;
      digitalWrite(PIN_D0, LOW);
      digitalWrite(PIN_D1, HIGH);
      Serial.println("------------- Comando para subida---------");

    }
    else if (temp == 'c' || temp == 'C')
    {
      estado = 3;
      digitalWrite(PIN_D0, HIGH);
      digitalWrite(PIN_D1, LOW);
      Serial.println("------------- Comando para acionamento ---------");
    }

    else if (temp ==  't') {
      bau.tare();                                // zera a balança
      Serial.println(" Balança zerada");             // imprime no monitor serial
    }
    else if (temp == 'p' || temp == 'P') {
      Serial.print("Peso: ");
      Serial.print(bau.get_units(), 2);         // imprime peso na balança com duas casas decimais
      Serial.println(" kg");                             // imprime no monitor serial
      delay(0) ;
    }
  }


  if (estado == 3) {
    Serial.print(millis());
    Serial.print(",");
    Serial.println(bau.get_units(), 2);         // imprime peso na balança com duas casas decimais
  }

  //verifica sensor

  if (digitalRead(5) == false&&estado==3) {
    estado = 1;
    digitalWrite(PIN_D0, LOW);
    digitalWrite(PIN_D1, LOW);
    Serial.println("------------- Comando segurança ---------");

  }

  

}
