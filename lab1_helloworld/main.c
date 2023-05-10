/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

#include <stdio.h>

#include "display.h"

#define TEXT_SIZE 2
#define CURSOR_X 10
#define CURSOR_Y (DISPLAY_HEIGHT / 2)
#define TRI_X0 ((DISPLAY_WIDTH / 2) - 25)
#define TRI_X1 ((DISPLAY_WIDTH / 2) + 25)
#define TRI_X2 ((DISPLAY_WIDTH / 2))
#define YPAD0 50
#define YPAD1 100
#define YPAD2 (DISPLAY_HEIGHT - 50)
#define YPAD3 (DISPLAY_HEIGHT - 100)
#define QUARTER_WIDTH (DISPLAY_WIDTH / 4)
#define THREE_QUARTER_WIDTH 240

// Print out "hello world" on both the console and the LCD screen.
int main() {

  // Initialize display driver, and fill scren with black
  display_init();
  display_fillScreen(DISPLAY_BLACK); // Blank the screen.

  display_drawLine(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_GREEN);
  display_drawLine(0, DISPLAY_HEIGHT, DISPLAY_WIDTH, 0, DISPLAY_GREEN);

  display_fillTriangle(TRI_X0, YPAD0, TRI_X1, YPAD0, TRI_X2, YPAD1,
                       DISPLAY_YELLOW);
  display_drawTriangle(TRI_X0, YPAD2, TRI_X1, YPAD2, TRI_X2, YPAD3,
                       DISPLAY_YELLOW);

  display_fillCircle(THREE_QUARTER_WIDTH, CURSOR_Y, 20, DISPLAY_RED);
  display_drawCircle(QUARTER_WIDTH, CURSOR_Y, 20, DISPLAY_RED);

  // Configure display text settings
  display_setTextColor(DISPLAY_RED); // Make the text red.
  display_setTextSize(TEXT_SIZE);    // Make the text a little larger.

  // Set the cursor location and print to the LCD
  display_setCursor(CURSOR_X, CURSOR_Y);
  // display_println("hello world (on the LCD)!");

  // Also print out 'hello world' on the terminal (stdout).
  printf("hello world!");
  return 0;
}
