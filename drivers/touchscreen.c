#include "touchscreen.h"
#include <math.h>
#include <stdint.h>
typedef enum { ea, eb, ec } ta;
volatile static uint32_t va;
volatile static bool vb;
static uint16_t x;
static uint16_t y;
static uint8_t z;
volatile static ta vc;
volatile static uint32_t vd;
void touchscreen_init(double aa) {
  vc = ea;
  vd = 0;
  va = (uint32_t)ceil(0.05 / aa);
  vb = false;
}
void touchscreen_tick() {
  switch (vc) {
  case ea:
    if (display_isTouched()) {
      display_clearOldTouchData();
      vc = eb;
    }
    break;
  case eb:
    if (vd == va) {
      if (!display_isTouched())
        vc = ea;
      else {
        vc = ec;
        display_getTouchedPoint(&x, &y, &z);
      }
    }
    break;
  case ec:
    if (!display_isTouched()) {
      vc = ea;
    }
    break;
  }
  switch (vc) {
  case ea:
    vd = 0;
    break;
  case eb:
    vd++;
    break;
  case ec:
    vb = true;
    break;
  }
}
touchscreen_status_t touchscreen_get_status() {
  if (vc == ec)
    return TOUCHSCREEN_PRESSED;
  else if (vb)
    return TOUCHSCREEN_RELEASED;
  else
    return TOUCHSCREEN_IDLE;
}
void touchscreen_ack_touch() { vb = false; }
display_point_t touchscreen_get_location() {
  display_point_t la = {x, y};
  return la;
}