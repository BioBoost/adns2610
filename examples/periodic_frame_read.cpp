#include "mbed.h"
#include "adns_2610.h"

Serial pc(USBTX, USBRX);
DigitalOut alive(LED1);

ADNS2610 opticalSensor;
uint8_t buffer[ADNS2610::NUMBER_OF_PIXELS];

void print_buffer(void) {
  printf("Frame:\n");
  for (unsigned int i = 0; i < sizeof(buffer); i++) {
    printf("|%d", buffer[i]);
  }
  printf("\n");
}

// main() runs in its own thread in the OS
int main(void) {
  pc.baud(115200);
  printf("Starting optical mouse sensoring app ...\n");
  printf("Status: %s\n", (opticalSensor.get_status() == 1 ? "awake" : "not responding"));

  while(true) {
    alive = !alive;
    wait(1.0);
    if (opticalSensor.read_frame(buffer)) {
      print_buffer();
    } else {
      printf("Something went wrong\n");
    }
  }
}