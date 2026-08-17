#include "neopixel.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

namespace
{
Adafruit_NeoPixel neoPixel(1, 48, NEO_GRB + NEO_KHZ800);
uint8_t colorPosition = 0;
}

void neopixelInit()
{
  neoPixel.begin();
  neoPixel.clear();
  neoPixel.show();
  Serial.println("NeoPixel ready");
}

void setNeopixel(int red, int green, int blue)
{
  neoPixel.setPixelColor(0, neoPixel.Color(red, green, blue));
  neoPixel.show();
}

void neopixelTest()
{
  int red;
  int green;
  int blue;

  if (colorPosition < 85)
  {
    red = 255 - colorPosition * 3;
    green = colorPosition * 3;
    blue = 0;
  }
  else if (colorPosition < 170)
  {
    const int phase = colorPosition - 85;
    red = 0;
    green = 255 - phase * 3;
    blue = phase * 3;
  }
  else
  {
    const int phase = colorPosition - 170;
    red = phase * 3;
    green = 0;
    blue = 255 - phase * 3;
  }

  setNeopixel(red, green, blue);
  colorPosition++;
}
