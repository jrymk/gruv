#include <SPI.h>
#include <ILI9488_t3.h>
#include <Wire.h>
#include <GT911.h>
#include <ILI9488_t3_font_ComicSansMS.h>
#include <touch.h>

#define INT_PIN 39
#define RST_PIN 3

#define TFT_RST 23
#define TFT_DC 9
#define TFT_CS 10
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);

int oldcolor, currentcolor;
int handlecounter = 0;

void setup()
{
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(180);

  Wire.setClock(400000);
  Wire.begin();
  delay(300);

  touch::touchPanel.begin(INT_PIN, RST_PIN, GT911_I2C_ADDR_BA, touch::touchPanelHandler);

  tft.fillScreen(ILI9488_BLACK);
}

int counter = 0;
int time = 0;

void loop()
{
  touch::touchPanel.loop();
  counter++;

  delay(1);

  Serial.println(touch::touchPanelCount);

  if (millis() - time >= 1000)
  {
    time += 1000;
    Serial.printf("%d %d\n", counter, handlecounter);
    counter = 0;
    handlecounter = 0;
  }
}