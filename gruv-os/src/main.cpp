#include <Arduino.h>
#include <Wire.h>
#include "./Arduino_GT911_Library/Goodix.h"

// pins below are defined on board LilyGo LilyPi, you can redefine it to the proper numeric values
#define INT_PIN 39
#define RST_PIN 3
#define LILYGO_LILYPI false //true only if config data is available

Goodix touch = Goodix();

int backlightBrightness = 0;

void handleTouch(int8_t contacts, GTPoint *points)
{
  Serial.printf("Contacts: %d\n", contacts);
  backlightBrightness = 204 * contacts;
  for (uint8_t i = 0; i < contacts; i++)
  {
    Serial.printf("C%d: %d %d \n", points[i].trackId, points[i].x, points[i].y);
  }
}

void touchStart()
{
  unsigned short configInfo;

  touch.begin(INT_PIN, RST_PIN, GOODIX_I2C_ADDR_BA);
  Serial.print("Check ACK on addr request on 0x");
  Serial.print(touch.i2cAddr, HEX);
  Wire.beginTransmission(touch.i2cAddr);
  if (!Wire.endTransmission())
  {
    Serial.println(": SUCCESS");
  }
  else
  {
    Serial.print(": ERROR!");
  }
  configInfo = touch.configCheck(LILYGO_LILYPI);
  if (!configInfo)
  {
    Serial.println("Config is OK");
  }
  else
  {
    Serial.print("Config ERROR: ");
    Serial.println(configInfo);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\nGoodix GT911x touch driver");

  Wire.setClock(400000);
  Wire.begin();
  delay(300);

  touch.setHandler(handleTouch);
  touchStart();
  pinMode(13, OUTPUT);
  analogWriteRes(10);
}

void loop()
{
  backlightBrightness -= 1;
  backlightBrightness = max(backlightBrightness, 0);
  analogWrite(13, backlightBrightness);
  touch.loop();
  delay(1);
}