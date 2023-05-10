#include "buttons.h"
#include "display.h"
#include "switches.h"
#include <stdio.h>

void gpioTest_buttons() {
  buttons_init();
  display_init();

  while (1) {
    uint8_t data = buttons_read();

    uint8_t btn0 = data & BUTTONS_BTN0_MASK;
    uint8_t btn1 = data & BUTTONS_BTN1_MASK;
    uint8_t btn2 = data & BUTTONS_BTN2_MASK;
    uint8_t btn3 = data & BUTTONS_BTN3_MASK;
    if (btn0) {
      display_fillScreen(DISPLAY_CYAN);
      printf("%s\n", "btn0");
    } else if (btn1) {
      display_fillScreen(DISPLAY_RED);
      printf("%s\n", "btn1");
    } else if (btn2) {
      display_fillScreen(DISPLAY_GREEN);
      printf("%s\n", "btn2");
    } else if (btn3) {
      display_fillScreen(DISPLAY_MAGENTA);
      printf("%s\n", "btn3");
    }
  }
}

void gpioTest_switches() {
  switches_init();
  uint8_t data = switches_read();

  uint8_t sw0 = SWITCHES_SW0_MASK;
  uint8_t sw1 = SWITCHES_SW1_MASK;
  uint8_t sw2 = SWITCHES_SW2_MASK;
  uint8_t sw3 = SWITCHES_SW3_MASK;
}