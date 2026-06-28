#include <Arduino.h>

#define SOIL_SENSOR_PIN   A3
#define PUMP_PIN          7

#define DRY_THRESHOLD     500
#define CHECK_DELAY_MS    1000

void setup()
{
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
}

void loop()
{
  int soilMoisture = analogRead(SOIL_SENSOR_PIN);

  if (soilMoisture > DRY_THRESHOLD)
  {
    digitalWrite(PUMP_PIN, HIGH); 
  }
  else
  {
    digitalWrite(PUMP_PIN, LOW);  
  }

  delay(CHECK_DELAY_MS);
}
