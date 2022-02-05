/***************************************************
  This is our touchscreen painting example for the Adafruit ILI9341
  captouch shield
  ----> http://www.adafruit.com/products/1947
  Check out the links above for our tutorials and wiring diagrams
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h> // this is needed for display
#include <ILI9488_t3.h>
#include <Wire.h> // this is needed for FT6206
#include <Goodix.h>

#define INT_PIN 39
#define RST_PIN 3
Goodix touch = Goodix();

#define TFT_RST 23
#define TFT_DC 9
#define TFT_CS 10
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

int handlecounter = 0;

void handleTouch(int8_t contacts, GTPoint *points)
{
  handlecounter++;
  //Serial.printf("Contacts: %d\n", contacts);
  for (uint8_t i = 0; i < contacts; i++)
  {
    //Serial.printf("C%d: %d %d \n", points[i].trackId, points[i].x, points[i].y);

    if (points[i].y < BOXSIZE)
    {
      oldcolor = currentcolor;

      if (points[i].x < BOXSIZE)
      {
        currentcolor = ILI9488_RED;
        tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
      }
      else if (points[i].x < BOXSIZE * 2)
      {
        currentcolor = ILI9488_YELLOW;
        tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
      }
      else if (points[i].x < BOXSIZE * 3)
      {
        currentcolor = ILI9488_GREEN;
        tft.drawRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
      }
      else if (points[i].x < BOXSIZE * 4)
      {
        currentcolor = ILI9488_CYAN;
        tft.drawRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
      }
      else if (points[i].x < BOXSIZE * 5)
      {
        currentcolor = ILI9488_BLUE;
        tft.drawRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
      }
      else if (points[i].x <= BOXSIZE * 6)
      {
        currentcolor = ILI9488_MAGENTA;
        tft.drawRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
      }

      if (oldcolor != currentcolor)
      {
        if (oldcolor == ILI9488_RED)
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9488_RED);
        if (oldcolor == ILI9488_YELLOW)
          tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9488_YELLOW);
        if (oldcolor == ILI9488_GREEN)
          tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9488_GREEN);
        if (oldcolor == ILI9488_CYAN)
          tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9488_CYAN);
        if (oldcolor == ILI9488_BLUE)
          tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9488_BLUE);
        if (oldcolor == ILI9488_MAGENTA)
          tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9488_MAGENTA);
      }
    }
    if (((points[i].y - PENRADIUS) > BOXSIZE) && ((points[i].y + PENRADIUS) < tft.height()))
    {
      tft.fillCircle(points[i].x, points[i].y, pow(1.2, points[i].area / 10), currentcolor);
    }
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
  configInfo = touch.configCheck(false);
  if (!configInfo)
  {
    Serial.println("Config is OK");
  }
  else
  {
    Serial.print("Config ERROR: ");
    Serial.println(configInfo);
  }
  touch.fwResolution(320, 480, 0);
  Serial.println(touch.configCheck(true));
}

void setup(void)
{
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  Serial.begin(115200);
  Serial.println(F("Cap Touch Paint!"));

  tft.begin();
  tft.setRotation(0); // 180

  Wire.setClock(800000);
  Wire.begin();
  delay(300);

  touch.setHandler(handleTouch);

  touchStart();

  Serial.println("Capacitive touchscreen started");

  tft.fillScreen(ILI9488_BLACK);

  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9488_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9488_YELLOW);
  tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9488_GREEN);
  tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9488_CYAN);
  tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9488_BLUE);
  tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9488_MAGENTA);

  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9488_WHITE);
  currentcolor = ILI9488_RED;
}

int counter = 0;
int time = 0;

void loop()
{

  // Print out raw data from screen touch controller
  /*
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print(" -> ");
 
// Maybe flip it around to match the screen.
//  p.x = map(p.x, 0, 240, 240, 0);
//  p.y = map(p.y, 0, 320, 320, 0);
  // Print out the remapped (rotated) coordinates
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
  */
  touch.loop();
  //delay(1);
  counter++;

  if (millis() - time >= 1000)
  {
    time += 1000;
    Serial.printf("%d %d\n", counter, handlecounter);
    counter = 0;
    handlecounter = 0;
  }
}