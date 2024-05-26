# pico-neopixel
PIO &amp; DMA based simple neopixel library for rp2040 microcontrollers
## Installation
Just clone the repo to your project
## Features
1. Simple and intuitive
2. Able to configure output pins on the fly
3. provide non-blocking function to free up CPU time for other purposes
4. PIO & DMA makes it possible to send multiple lanes at the same time
5. Support up to 8 channels and unlimited number of strips (well actually limited by the number of GPIOs)
## Examples
- **simple** An example to show red color on a single strip
- **pin_change** An example to demonstrate how to control multiple lanes using one channel by changing the ouput pin on the fly
- **non_blocking** An example that uses non-blocking version of set_pixels
- **8_channels** An example to demonstrate how to send multiple lanes using 8 channels simultaneously
  
**Note**: In the **non_blocking** and **8_channels** example, after confirming that DMA has completed the data transfer, we used the delayMicroseconds(400) to ensure that PIO has sent out the last 24-bit data. However, if you are confident that the interval between two data loadings is sufficient for PIO to complete the transmission, this delay is actually unnecessary. For example, it takes about 3 to 4 milliseconds to complete the transmission of 100 RGBs on a single channel
