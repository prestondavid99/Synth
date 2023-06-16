#include "interrupts.h"
#include "armInterrupts.h"
#include "xil_io.h"
#include "xparameters.h"
volatile uint32_t interrupt_occurred = 0;
static void (*va[32])() = {NULL};
static uint32_t fa(uint32_t x) {
  return Xil_In32(XPAR_AXI_INTC_0_BASEADDR + x);
}
static void fb(uint32_t x, uint32_t y) {
  Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + x, y);
}
static void fc(uint8_t x) { fb(0x0C, x); }
static void fd() {
  interrupt_occurred = 1;
  uint32_t x = fa(0x04);
  for (uint32_t i = 0; i < 32; i++) {
    if (x & (1 << i)) {
      if (va[i])
        va[i]();
      fc(1 << i);
    }
  }
}
void interrupts_init() {
  armInterrupts_init();
  armInterrupts_setupIntc(fd);
  armInterrupts_enable();
  fb(0x1C, 0x01 | 0x02);
  interrupts_irq_disable(0xFF);
}
void interrupts_register(uint8_t x, void (*y)()) { va[x] = y; }
void interrupts_irq_enable(uint8_t x) { fb(0x10, 1 << x); }
void interrupts_irq_disable(uint8_t x) { fb(0x14, 1 << x); }