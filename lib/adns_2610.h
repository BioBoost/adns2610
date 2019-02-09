#pragma once

#include "mbed.h"

class ADNS2610 {

  private:
    enum Register {
      CONFIG = 0x00,
      STATUS = 0x01,
      PIXEL_DATA = 0x08
    };

    const static uint8_t NO_SLEEP_MASK = 0x01;
    const static uint8_t PID_MASK = 0xE0;

  public:
    ADNS2610(PinName clk=D7, PinName dio=D6);

  public:
    void reset(void);
    uint8_t get_status(void);

  private:
    void disable_sleep(void);

  private:
    uint8_t read_register(Register address);
    void write_register(Register address, uint8_t data);

  private:
    uint8_t read(void);
    void write(uint8_t data);

  private:
    DigitalOut clock;
    DigitalInOut dataio;
};