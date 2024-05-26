/* 
  It is recommended to see non_blocking example first
*/

#include "neopixel.h"

#define NUMBER_OF_CHANNELS 8  // this library can support up to 8 channels
#define NUMBER_OF_RGBS 10  // number of RGBs
#define BRIGHTNESS 255  // brightness (from 0-255)
const int RGB_PINS[NUMBER_OF_CHANNELS] = {0, 1, 2, 3, 4, 5, 6, 7};

neopixel channel[NUMBER_OF_CHANNELS];  // rgb output channel instance

uint32_t strip_color[NUMBER_OF_RGBS];  // buffer that holds the RGB data

void setup()
{
  Serial.begin(115200);

  for (int i=0;i<NUMBER_OF_CHANNELS;i++)  // initialize the channels
  {
    channel[i].init(RGB_PINS[i]);
  }

  for (int i=0;i<NUMBER_OF_RGBS;i++)  // set all colors to red
  {                              //R   G   B   W
    strip_color[i] = channel[0].urgb_u32(255, 0, 0, BRIGHTNESS);
  }
}

void loop()
{
  // load data into 8 channels and trigger them
  // all channels sending simultaneously
  for (int i=0;i<NUMBER_OF_CHANNELS;i++)
  {
    channel[i].set_pixels(strip_color, NUMBER_OF_RGBS);
  }
  
  // wait until all channels have finished sending
  while(channel[0].dma_busy() || channel[1].dma_busy() ||
        channel[2].dma_busy() || channel[3].dma_busy() ||
        channel[4].dma_busy() || channel[5].dma_busy() ||
        channel[6].dma_busy() || channel[7].dma_busy()); {}
  
  // make sure PIO SMs have finished sending
  delayMicroseconds(400);
}