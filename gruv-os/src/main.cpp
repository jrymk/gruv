#include <SPI.h>
#include <Wire.h>
#include <GT911.h>
#include <touch.h>
#include <string>
#include <Adafruit_GFX.h>
#include <ILI9488p.h>

#define INT_PIN 16
#define RST_PIN 17

ILI9488p tft;

uint32_t ID;

int oldcolor, currentcolor;
int handlecounter = 0;

extern float tempmonGetTemp(void);

unsigned long testFillScreen()
{
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}

unsigned long testText(uint32_t ID)
{
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print("TFT is ");
  tft.println(ID, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color)
{
  unsigned long start, t;
  int x1, y1, x2, y2,
      w = tft.width(),
      h = tft.height();

  tft.fillScreen(BLACK);

  x1 = y1 = 0;
  y2 = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  t = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(BLACK);

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  t += micros() - start;

  tft.fillScreen(BLACK);

  x1 = 0;
  y1 = h - 1;
  y2 = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  t += micros() - start;

  tft.fillScreen(BLACK);

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(BLACK);
  start = micros();
  for (y = 0; y < h; y += 5)
    tft.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color)
{
  unsigned long start;
  int n, i, i2,
      cx = tft.width() / 2,
      cy = tft.height() / 2;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2)
{
  unsigned long start, t = 0;
  int n, i, i2,
      cx = tft.width() / 2 - 1,
      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color)
{
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(BLACK);
  start = micros();
  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color)
{
  unsigned long start;
  int x, y, r2 = radius * 2,
            w = tft.width() + radius,
            h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2)
  {
    for (y = 0; y < h; y += r2)
    {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles()
{
  unsigned long start;
  int n, i, cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5)
  {
    tft.drawTriangle(
        cx, cy - i,     // peak
        cx - i, cy + i, // bottom left
        cx + i, cy + i, // bottom right
        NAVY);
  }

  return micros() - start;
}

unsigned long testFilledTriangles()
{
  unsigned long start, t = 0;
  int i, cx = tft.width() / 2 - 1,
         cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5)
  {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     BLACK);
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     MAROON);
  }

  return t;
}

unsigned long testRoundRects()
{
  unsigned long start;
  int w, i, i2,
      cx = tft.width() / 2 - 1,
      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  w = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i < w; i += 6)
  {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, PINK);
  }

  return micros() - start;
}

unsigned long testFilledRoundRects()
{
  unsigned long start;
  int i, i2,
      cx = tft.width() / 2 - 1,
      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
  {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, DARKCYAN);
  }

  return micros() - start;
}

void setup()
{
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  Serial.begin(115200);

  Wire.setClock(4000000);
  Wire.begin();
  delay(300);

  touch::touchPanel.begin(INT_PIN, RST_PIN, GT911_I2C_ADDR_BA, touch::touchPanelHandler);

  touch::touchPanel.printConfig();

  ID = tft.readID();
  tft.setResolution(320, 480); // Set your resolution
  Serial.print("Device ID: 0x");
  Serial.println(ID, HEX);
  tft.begin(ID);

  uint32_t width = tft.width();
  Serial.print("Width: ");
  Serial.println(width);
  uint32_t height = tft.height();
  Serial.print("Height: ");
  Serial.println(height);

  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());

  Serial.print(F("Text                     "));
  Serial.println(testText(ID));

  Serial.print(F("Lines                    "));
  Serial.println(testLines(CYAN));

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(RED, BLUE));

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());

  Serial.println(F("Done!"));
}

int time = 0;

int prevTime = 0;
int loopRate = 0;

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

        // tft.fillRect(0, y + i * 1, 320, 1, tft.color565(128 * sin(i / 400.0 * PI * 2) + 127, 128 * sin((i / 400.0 + 0.333333) * PI * 2) + 127, 128 * sin((i / 400.0 + 0.666667) * PI * 2) + 127));
      }
      for (int i = 0; i < 2000; i++)
      {
        if (y + i * 1 < 10)
          continue;
        if (y + i * 1 + 1 > 480)
          continue;

        if (i % 20 == 0)
        {
          tft.setTextColor(BLACK);
          tft.setCursor(20, y + i * 1);
          tft.printf("%d", i);
        }
      }
      tft.fillCircle(50, y, 10, WHITE);
      tft.fillRect(0, y + 100 * 20, 320, 20, BLACK);

      tft.setTextColor(BLACK);
      tft.setCursor(100, 240);
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

        tft.fillRect(0, y + i * 1, 320 * 1.0, 1, tft.color565(128 * sin(i / 400.0 * PI * 2) + 127, 128 * sin((i / 400.0 + 0.333333) * PI * 2) + 127, 128 * sin((i / 400.0 + 0.666667) * PI * 2) + 127));
      }
      for (int i = 0; i < 2000; i++)
      {
        if (y + i * 1 < 10)
          continue;
        if (y + i * 1 + 1 > 480)
          continue;

        if (i % 20 == 0)
        {
          tft.setTextColor(BLACK);
          tft.setCursor(20, y + i * 1);
          tft.printf("%d", i);
        }
      }
      tft.fillCircle(50, y, 10, WHITE);
      tft.fillRect(0, y + 100 * 20, 320, 20, BLACK);

      tft.setTextColor(BLACK);
      tft.setCursor(100, 240);
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

    tft.fillRect(0, 0, 320, 10, BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(0, 0);
    if (loopRate >= 10)
      tft.printf("loop(): %dus (%dHz)\n", loopRate, (int)(1000000. / loopRate));
    else
      tft.printf("loop(): %dus\n", loopRate);

    tft.setCursor(175, 0);
    tft.printf("%dMHz", F_CPU_ACTUAL / 1000000);

    tft.setCursor(277, 0);
    uint16_t tempColor = MAGENTA;
    if (tempmonGetTemp() <= 32)
      tempColor = GREEN;
    else if (tempmonGetTemp() <= 45)
      tempColor = YELLOW;
    else if (tempmonGetTemp() <= 55)
      tempColor = ORANGE;
    else
      tempColor = RED;
    tft.setTextColor(tempColor);
    tft.printf("%.2f C", tempmonGetTemp());
    tft.drawCircle(309, 2, 2, tempColor);
    tft.drawFastHLine(0, 10, 320, GRAY);
  }

  // tft.updateScreen();
}