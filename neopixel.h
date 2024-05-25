#include "ws2812.pio.h"
#include "hardware/pio.h"

#define IS_RGBW false

class neopixel {
private:
  PIO pio;
  int sm;
  int dma_channel;
  bool running;
  int init_pin;

  void put_pixel_blocking(uint32_t pixel_grb);

public:
  neopixel();

  int init(int pin);
  int set_pin(int pin);
  int set_pixels_blocking(uint32_t* buf, int size);
  int dma_busy();
  int set_pixels(uint32_t* buf, int size);
  uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  uint32_t urgb_u32_dma(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
};