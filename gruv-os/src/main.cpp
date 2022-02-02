#include <Arduino.h>
#include <HardwareSerial.h>

void setup()
{
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  analogWriteResolution(10);
}

void loop()
{
  // put your main code here, to run repeatedly:
  analogWrite(13, 127);
  delay(500);
  analogWrite(13, 511);
  delay(500);
}