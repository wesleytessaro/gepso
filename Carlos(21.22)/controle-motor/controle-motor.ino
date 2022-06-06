#include <AccelStepper.h>
// Define a stepper motor 3 for arduino
// direction Digital 9 (CW), pulses Digital 8 (CLK)
AccelStepper stepper(3, 8, 9);

void setup()
{
  // Change these to suit your stepper if you want
  Serial.begin(9600);
  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);

  //pinMode(3,OUTPUT);
 //pinMode(8,OUTPUT);
 // pinMode(9,OUTPUT);
  stepper.setMaxSpeed(2500);//2600
  stepper.setAcceleration(100);
  //stepper.moveTo(24100);
}

void loop()
{

  if (buscarEstado()==1){//Motor desligado
    stepper.stop();
    Serial.println("Desligar");
  }
  else if (buscarEstado()==2){ //Subir motor
   stepper.setSpeed(-2000);
    stepper.runSpeed();
    Serial.println("subir");
  }
  else if (buscarEstado()==3){ //Descer motor
      stepper.setSpeed(800);
    stepper.runSpeed();
    Serial.println("Descer");
  }

delay(0);
}

int buscarEstado (){

  if (digitalRead(6)==LOW && digitalRead(7)==HIGH ){
    return 2;
  }
  else if (digitalRead(6)==HIGH && digitalRead(7)==LOW ) {
    return 3;
   }
  else if (digitalRead(6)==LOW && digitalRead(7)==LOW ) {
   return 1; 
   }
  
}
