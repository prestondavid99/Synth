#include "buttons.h"
#include "display.h"
#include "leds.h"
#include "switches.h"
#include <stdio.h>

#define RECT_WIDTH 80
#define RECT_HEIGHT 120
#define CURSOR_Y 50
#define RECT_Y 0
#define TEXT_SIZE 2

#define RECT3_X 0
#define RECT2_X 80
#define RECT1_X 160
#define RECT0_X 240
#define CURSOR3_X (RECT3_X + 20)
#define CURSOR2_X (RECT2_X + 20)
#define CURSOR1_X (RECT1_X + 20)
#define CURSOR0_X (RECT0_X + 20)
#define ALL_SIGNALS 0x0F

/*This function configures the text on the screen by establishing the string
  to be printed, the x-position on the screen, and the color*/
void setText(char str[], int16_t x, uint16_t color) {
  display_setCursor(x, CURSOR_Y);
  display_setTextColor(color);
  display_setTextSize(TEXT_SIZE);
  display_println(str);
}

/*This function tests the buttons on the ZYBO board by drawing rectangles on
  the screen. All buttons pressed will exit the loop*/
void gpioTest_buttons() {
  buttons_init();
  display_init();
  bool btn0isPressed = false;
  bool btn1isPressed = false;
  bool btn2isPressed = false;
  bool btn3isPressed = false;

  while (buttons_read() !=
         ALL_SIGNALS) { // This loops until all buttons are pressed
    uint8_t data = buttons_read();
    uint8_t btn0 = data & BUTTONS_BTN0_MASK;
    uint8_t btn1 = data & BUTTONS_BTN1_MASK;
    uint8_t btn2 = data & BUTTONS_BTN2_MASK;
    uint8_t btn3 = data & BUTTONS_BTN3_MASK;

    if (btn0 && !btn0isPressed) { // if btn0 is read through the data AND btn0
                                  // is not pressed
      display_fillRect(RECT0_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_WHITE);
      setText("BTN0", CURSOR0_X, DISPLAY_BLACK);
      btn0isPressed = true;
    } else if (!btn0) { // Otherwise, remove the rectangle by coloring it black
      display_fillRect(RECT0_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_BLACK);
      btn0isPressed = false;
    }
    if (btn1 && !btn1isPressed) { // if btn1 is read through the data AND btn1
                                  // is not pressed
      display_fillRect(RECT1_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_CYAN);
      setText("BTN1", CURSOR1_X, DISPLAY_BLACK);
      btn1isPressed = true;
    } else if (!btn1) { // Otherwise, remove the rectangle by coloring it black
      display_fillRect(RECT1_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_BLACK);
      btn1isPressed = false;
    }
    if (btn2 && !btn2isPressed) { // if btn2 is read through the data AND btn2
                                  // is not pressed
      display_fillRect(RECT2_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_RED);
      setText("BTN2", CURSOR2_X, DISPLAY_WHITE);
      btn2isPressed = true;
    } else if (!btn2) { // Otherwise, remove the rectangle by coloring it black
      display_fillRect(RECT2_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_BLACK);
      btn2isPressed = false;
    }
    if (btn3 && !btn3isPressed) { // if btn3 is read through the data AND btn3
                                  // is not pressed
      display_fillRect(RECT3_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_BLUE);
      setText("BTN3", CURSOR3_X, DISPLAY_WHITE);
      btn3isPressed = true;
    } else if (!btn3) { // Otherwise, remove the rectangle by coloring it black
      display_fillRect(RECT3_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT, DISPLAY_BLACK);
      btn3isPressed = false;
    }
  }
  display_fillScreen(DISPLAY_BLACK);
}

/*This function will test the switches on the ZYBO board. All switches flipped
  will end the loop.*/
void gpioTest_switches() {
  switches_init();
  leds_init();

  while (switches_read() !=
         ALL_SIGNALS) { // Read the data until all switches are flipped.
    leds_write(switches_read());
  }
  leds_write(0);
}
