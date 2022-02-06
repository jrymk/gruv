#include <SPI.h>
#include <ILI9488_t3.h>
#include <Wire.h>
#include <GT911.h>
#include <ili9488_t3_font_Arial.h>
#include <ili9488_t3_font_ArialBold.h>
#include <touch.h>
#include <string>

#define INT_PIN 39
#define RST_PIN 3

#define TFT_RST 23
#define TFT_DC 9
#define TFT_CS 10
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);

int oldcolor, currentcolor;
int handlecounter = 0;

extern float tempmonGetTemp(void);

void setup()
{
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  Serial.begin(115200);

  tft.begin(40000000u);
  tft.setRotation(180);

  Wire.setClock(4000000);
  Wire.begin();
  delay(300);

  touch::touchPanel.begin(INT_PIN, RST_PIN, GT911_I2C_ADDR_BA, touch::touchPanelHandler);

  touch::touchPanel.printConfig();

  tft.fillScreen(ILI9488_BLACK);

  tft.useFrameBuffer(true);
  tft.updateScreenAsync(true);
}

int time = 0;

int prevTime = 0;
int loopRate = 0;

uint16_t colors[] = {
    ILI9488_RED,
    // ILI9488_ORANGE,
    ILI9488_YELLOW,
    ILI9488_GREEN,
    ILI9488_CYAN,
    ILI9488_BLUE,
    ILI9488_MAGENTA};

int y = 0;

void loop()
{
  touch::update();

  loopRate = (micros() - prevTime);
  prevTime = micros();

  while (!touch::trackEvents.empty())
  {
    auto event = touch::trackEvents.front();
    touch::trackEvents.pop();
    if (event.type == touch::TrackEventType::RELEASE)
    {
      /*tft.fillRect(min(touch::trackPoints[event.trackId].pressPos.x, touch::trackPoints[event.trackId].releasePos.x),
                   min(touch::trackPoints[event.trackId].pressPos.y, touch::trackPoints[event.trackId].releasePos.y),
                   abs(touch::trackPoints[event.trackId].releasePos.x - touch::trackPoints[event.trackId].pressPos.x),
                   abs(touch::trackPoints[event.trackId].releasePos.y - touch::trackPoints[event.trackId].pressPos.y),
                   colors[event.trackId]);*/
    }
    if (event.type == touch::TrackEventType::PRESS || event.type == touch::TrackEventType::RELEASE)
    {
      for (int i = 0; i < 2000; i++)
      {
        if (y + i * 1 < 10)
          continue;
        if (y + i * 1 + 1 > 480)
          continue;

        tft.fillRect(0, y + i * 1, 320, 1, tft.color565(128 * sin(i / 400.0 * PI * 2) + 127, 128 * sin((i / 400.0 + 0.333333) * PI * 2) + 127, 128 * sin((i / 400.0 + 0.666667) * PI * 2) + 127));
      }
      for (int i = 0; i < 2000; i++)
      {
        if (y + i * 1 < 10)
          continue;
        if (y + i * 1 + 1 > 480)
          continue;

        if (i % 20 == 0)
        {
          tft.setTextColor(ILI9488_BLACK);
          tft.setCursor(20, y + i * 1);
          tft.setFont(Arial_14_Bold);
          tft.printf("%d", i);
        }
      }
      tft.fillCircle(50, y, 10, ILI9488_WHITE);
      tft.fillRect(0, y + 100 * 20, 320, 20, ILI9488_BLACK);

      tft.setTextColor(ILI9488_BLACK);
      tft.setCursor(100, 240);
      tft.setFont(Arial_48_Bold);
      tft.printf("%d", y);
    }
    if (event.type == touch::TrackEventType::POSITION)
    {
      y += touch::trackPoints[event.trackId].delta.y;

      if (y > 0)
        y = 0;
      y = max(-1520, y);

      for (int i = 0; i < 2000; i++)
      {
        if (y + i * 1 < 10)
          continue;
        if (y + i * 1 + 1 > 480)
          continue;

        tft.fillRect(0, y + i * 1, 320, 1, tft.color565(128 * sin(i / 400.0 * PI * 2) + 127, 128 * sin((i / 400.0 + 0.333333) * PI * 2) + 127, 128 * sin((i / 400.0 + 0.666667) * PI * 2) + 127));
      }
      for (int i = 0; i < 2000; i++)
      {
        if (y + i * 1 < 10)
          continue;
        if (y + i * 1 + 1 > 480)
          continue;

        if (i % 20 == 0)
        {
          tft.setTextColor(ILI9488_BLACK);
          tft.setCursor(20, y + i * 1);
          tft.setFont(Arial_14_Bold);
          tft.printf("%d", i);
        }
      }
      tft.fillCircle(50, y, 10, ILI9488_WHITE);
      tft.fillRect(0, y + 100 * 20, 320, 20, ILI9488_BLACK);

      tft.setTextColor(ILI9488_BLACK);
      tft.setCursor(100, 240);
      tft.setFont(Arial_48_Bold);
      tft.printf("%d", y);
    }
  }

  for (int i = 0; i < TOUCH_MAX_POINTS; i++)
  {
    touch::TrackPoint tp = touch::trackPoints[i];
    if (tp.enabled)
    {
      // tft.fillCircle(tp.pos.x, tp.pos.y, 100, colors[i]);
    }
  }

  // end
  if (millis() - time >= 200)
  {
    time += 200;
    handlecounter = 0;

    tft.fillRect(0, 0, 320, 10, ILI9488_BLACK);
    tft.setTextColor(ILI9488_WHITE);
    tft.setCursor(0, 0);
    tft.setFont(Arial_8_Bold);
    if (loopRate >= 10)
      tft.printf("loop(): %dus (%dHz)\n", loopRate, (int)(1000000. / loopRate));
    else
      tft.printf("loop(): %dus\n", loopRate);

    tft.setCursor(175, 0);
    tft.printf("%dMHz", F_CPU_ACTUAL / 1000000);

    tft.setCursor(280, 0);
    uint16_t tempColor = ILI9488_MAGENTA;
    if (tempmonGetTemp() <= 32)
      tempColor = ILI9488_GREEN;
    else if (tempmonGetTemp() <= 45)
      tempColor = ILI9488_YELLOW;
    else if (tempmonGetTemp() <= 55)
      tempColor = ILI9488_ORANGE;
    else
      tempColor = ILI9488_RED;
    tft.setTextColor(tempColor);
    tft.printf("%.2f  C", tempmonGetTemp());
    tft.drawCircle(309, 2, 2, tempColor);
    tft.drawFastHLine(0, 10, 320, ILI9488_LIGHTGREY);
  }

  // tft.updateScreen();
}