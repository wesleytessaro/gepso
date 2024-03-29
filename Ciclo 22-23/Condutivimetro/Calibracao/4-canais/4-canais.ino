const float ArduinoVoltage = 5.00; // CHANGE THIS FOR 3.3v Arduinos
const float ArduinoResolution = ArduinoVoltage / 1024;
const float resistorValue = 10000.0;

int threshold = 3;
int inputPin = A0;
int ouputPin = A5;

void setup()
{
  Serial.begin(9600);
  pinMode(ouputPin, OUTPUT);
  pinMode(inputPin, INPUT);
}

void loop()
{
  int analogValue = 0;
  int oldAnalogValue = 1000;
  float returnVoltage = 0.0;
  float resistance = 0.0;
  double Siemens;
  float TDS = 0.0;

  while (((oldAnalogValue - analogValue) > threshold) || (oldAnalogValue < 50))
  {
    oldAnalogValue = analogValue;
    digitalWrite( ouputPin, HIGH );
    delay(10); // allow ringing to stop
    analogValue = analogRead( inputPin );
    digitalWrite( ouputPin, LOW );
  }

  Serial.print("Return voltage = ");
  returnVoltage = analogValue * ArduinoResolution;
  Serial.print(returnVoltage);
  Serial.println(" volts");


  Serial.print("That works out to a resistance of ");
  resistance = ((5.00 * resistorValue) / returnVoltage) - resistorValue;
  Serial.print(resistance);
  Serial.println(" Ohms.");


  Serial.print("Which works out to a conductivity of ");
  Siemens = 1.0 / (resistance / 1000000);
  Serial.print(Siemens);
  Serial.println(" microSiemens.");
  Serial.print("We can estimate Total Dissolved Solids to be on the order of ");
  TDS = 500 * (Siemens / 1000);
  Serial.print(TDS);
  Serial.println(" PPM.");
  if (returnVoltage > 4.9) Serial.println("Are you sure this isn't metal?");


  delay(5000);
}
