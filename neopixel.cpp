#include "ws2812.pio.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "neopixel.h"

neopixel::neopixel()
  : pio(pio0), sm(0), dma_channel(0), running(false), init_pin(0) {}

int neopixel::init(int pin) {
  if (running) return 0;
  init_pin = pin;
  sm = pio_claim_unused_sm(pio, false);  // Find a free SM
  if (sm < 0) {
    pio = pio1;  // Try pio1 if SM not found
    sm = pio_claim_unused_sm(pio, false);
  }
  if (sm < 0) return -1;  // Return error if SM not found
  dma_channel = dma_claim_unused_channel(false);  // Find a free DMA channel
  if (dma_channel < 0) return -1;  // Return error if DMA channel not found

  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, init_pin, 800000, IS_RGBW);

  dma_channel_config channel_config = dma_channel_get_default_config(dma_channel);
  channel_config_set_dreq(&channel_config, pio_get_dreq(pio, sm, true));
  channel_config_set_transfer_data_size(&channel_config, DMA_SIZE_32);
  channel_config_set_read_increment(&channel_config, true);
  dma_channel_configure(dma_channel,
                        &channel_config,
                        &pio->txf[sm],
                        NULL,
                        0,
                        false);
  running = true;
  return 0;
}

int neopixel::set_pin(int pin) {
  if (!running) return -1;
  pio_gpio_init(pio, pin);
  pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
  pio_sm_set_sideset_pins(pio, sm, pin);
  return 0;
}

void neopixel::put_pixel_blocking(uint32_t pixel_grb) {
  pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

int neopixel::set_pixels_blocking(uint32_t* buf, int size) {
  if (!running) return -1;
  for (int i = 0; i < size; i++)
    put_pixel_blocking(buf[i]);
  return 0;
}

int neopixel::dma_busy()
{
  return dma_channel_is_busy(dma_channel);
}

int neopixel::set_pixels(uint32_t* buf, int size) {
  if (!running) return -1;
  if (dma_busy()) return -1;

  dma_channel_set_trans_count(dma_channel, size, false);
  dma_channel_set_read_addr(dma_channel, (void*)buf, true);
  return 0;
}

uint32_t neopixel::urgb_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  uint32_t new_r = r;
  uint32_t new_g = g;
  uint32_t new_b = b;
  new_r = new_r * w / 255;
  new_g = new_g * w / 255;
  new_b = new_b * w / 255;
  return (new_r << 8) | (new_g << 16) | new_b;
}

uint32_t neopixel::urgb_u32_dma(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return urgb_u32(r, g, b, w) << 8;
}