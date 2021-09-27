#include <Arduino.h>

#define TRIGGER_ABOVE 2
#define ECHO_ABOVE 3

#define ECHO_BELOW 4
#define TRIGGER_BELOW 5

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop()
{
  delay(1000);
  Serial.println("Start");
  // put your main code here, to run repeatedly:
}