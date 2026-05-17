#include "neopixel.h"
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixel(1, 48, NEO_GRB + NEO_KHZ800);
int rgbState = 0;
void neopixelInit()
{
  //NEOPIXEL
  pixel.begin();
  pixel.show();
  Serial.println("NeoPixel ready");
}
void setNeopixel(int r, int g, int b)
{
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}
void neopixelTest()
{
  int r, g, b;
  if (rgbState < 85)
  {
    r = 255 - rgbState * 3;
    g = rgbState * 3;
    b = 0;
  }
  else if (rgbState < 170)
  {
    int temp = rgbState - 85;
    r = 0;
    g = 255 - temp * 3;
    b = temp * 3;
  }
  else
  {
    int temp = rgbState - 170;
    r = temp * 3;
    g = 0;
    b = 255 - temp * 3;
  }
  setNeopixel(r, g, b);
  rgbState++;
  if (rgbState > 255)
  {
    rgbState = 0;
  }
}