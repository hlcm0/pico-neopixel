#include "neopixel.h"

#define NUMBER_OF_RGBS 10  // number of RGBs
#define RGB_PIN_0 0  // output pin for RGB strip
#define RGB_PIN_1 1
#define BRIGHTNESS 255  // brightness (from 0-255)

neopixel rgb;  // rgb output channel instance

uint32_t strip_color[NUMBER_OF_RGBS];  // buffer that holds the RGB data

void setup()
{
  Serial.begin(115200);
  rgb.init(RGB_PIN_0);  // initialize the rgb
  for (int i=0;i<10;i++)  // set all colors to red
  {                              //R   G   B   W
    strip_color[i] = rgb.urgb_u32(255, 0, 0, BRIGHTNESS);
  }
}

void loop()
{
  rgb.set_pin(RGB_PIN_0);  // change output pin to RGB_PIN_0
  rgb.set_pixels_blocking(strip_color, NUMBER_OF_RGBS);  // send data to rgb strip
                                                         // block until finish
  rgb.set_pin(RGB_PIN_1);
  rgb.set_pixels_blocking(strip_color, NUMBER_OF_RGBS);
}