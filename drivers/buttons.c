#include "buttons.h"
#include "xil_io.h"
#include "xparameters.h"

static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_PUSH_BUTTONS_BASEADDR + offset);
}

static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_PUSH_BUTTONS_BASEADDR + offset, value);
}

void buttons_init() { writeRegister(4, 0x0F); }

uint8_t buttons_read() { return readRegister(0); }