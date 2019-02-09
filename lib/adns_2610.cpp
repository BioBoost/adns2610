#include "adns_2610.h"

ADNS2610::ADNS2610(PinName clk, PinName dio)
  : clock(clk), dataio(dio) {

  dataio.mode(PullUp);
  reset();
}

void ADNS2610::reset(void) {
  clock = 1;
  wait_us(5);
  clock = 0;
  wait_us(1);
  clock = 1;
  wait_ms(1000);
  disable_sleep();
}

void ADNS2610::disable_sleep(void) {
  write_register(CONFIG, NO_SLEEP_MASK);
}

void ADNS2610::write_register(Register address, uint8_t data) {
  uint8_t writeAddress = (uint8_t(address)) | 0x80; // Setting MSB = write

  write(writeAddress);
  write(data);
}

uint8_t ADNS2610::read_register(Register address) {
  write((uint8_t)address);
  uint8_t data = read();
  wait_us(110);
  return data;
}

void ADNS2610::write(uint8_t data) {
  dataio.output();
  for (unsigned int i = 8; i > 0; i--) {
    clock = 0;
    dataio = data & (1 << (i-1));
    clock = 1;
  }
}

uint8_t ADNS2610::read(void) {
  dataio.input();
  wait_us(110);  // Chip needs at least 100us to get the data ready

  uint8_t data = 0;
  for (unsigned int i = 8; i > 0; i--) {
    clock = 0;
    clock = 1;                        

    data |= (dataio.read() << (i-1));
  }
  return data;
}

uint8_t ADNS2610::get_status(void) {
  return read_register(STATUS);
}

// ADNS2610 dumps a 324-byte array, make sure buffer is large enough
// Pixel data byte: [SOF valid? 0-63]
bool ADNS2610::read_frame(uint8_t * buffer) {
  // Request frame
  write_register(PIXEL_DATA, REQ_PIXEL_DATA);

  Timer timer;
  timer.start();

  unsigned int i = 0;
  while (i < NUMBER_OF_PIXELS && timer.read_ms() < 1000) {
    uint8_t pixel = read_register(PIXEL_DATA);

    // Only continue if pixel data is valid
    if((pixel & 64)) {
      // Stop if receiving start of frame and we already are past first pixel
      if ((pixel & 128) && i != 0) {
        break;
      }
      buffer[i] = pixel & 63;
      i++;
    }
  }
  timer.stop();
  return (i == NUMBER_OF_PIXELS && timer.read_ms() < 1000);
}