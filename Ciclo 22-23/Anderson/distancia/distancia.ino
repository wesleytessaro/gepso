#include "Adafruit_VL53L0X.h"


Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup () {
  lox.begin();
  Serial.begin(115200);
  
}
void loop(){
  getDistancia();
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
