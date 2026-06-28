#include <Servo.h>

#define EAST_LDR_PIN    0
#define WEST_LDR_PIN    1
#define SERVO_PIN       9

// Constants
#define CALIBRATION       600
#define ERROR_THRESHOLD   15
#define SERVO_MIN         20
#define SERVO_MAX         150
#define SERVO_START       90
#define NIGHT_THRESHOLD   350
#define SERVO_DELAY_MS    100

Servo servo;
int east = 0;
int west = 0;
int error = 0;
int servoposition = SERVO_START;

void setup()
{
  servo.attach(SERVO_PIN);
}

void loop()
{
  east = CALIBRATION + analogRead(EAST_LDR_PIN);
  west = analogRead(WEST_LDR_PIN);

  if (east < NIGHT_THRESHOLD && west < NIGHT_THRESHOLD)
  {
    while (servoposition <= SERVO_MAX)
    {
      servoposition++;
      servo.write(servoposition);
      delay(SERVO_DELAY_MS);
    }
  }

  error = east - west;

  if (error > ERROR_THRESHOLD)
  {
    if (servoposition <= SERVO_MAX)
    {
      servoposition++;
      servo.write(servoposition);
    }
  }
  else if (error < -ERROR_THRESHOLD)
  {
    if (servoposition > SERVO_MIN)
    {
      servoposition--;
      servo.write(servoposition);
    }
  }

  delay(SERVO_DELAY_MS);
}
