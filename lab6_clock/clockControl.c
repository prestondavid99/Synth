#include "clockDisplay.h"
#include "touchscreen.h"
#include "xparameters.h"
#include <stdbool.h>
#include <stdio.h>

// All printed messages for states are provided here.
#define INIT_ST_MSG "init state\n"
#define WAITING_ST_MSG "waiting_st\n"
#define INC_DEC_ST_MSG "inc_dec_st\n"
#define LONG_PRESS_DELAY_ST "long_press_delay_st\n"
#define FAST_UPDATE_ST_MSG "fast_update_st\n"
#define HALF 2
#define TENTH 10

/*Setup States*/
enum clockControl_st_t {
  init_st, // Start here, transition out of this state on the first tick.
  waiting_st,
  inc_dec_st,
  long_press_delay_st,
  fast_update_st
};
static enum clockControl_st_t currentState;
static uint16_t delay_cnt = 0;
static uint16_t update_cnt = 0;
static uint64_t delay_num_ticks = 0;
static uint64_t update_num_ticks = 0;

/*Initialize the Clock*/
void clockControl_init(double period_s) {
  delay_num_ticks = ((1 / period_s) / HALF);
  update_num_ticks = ((1 / period_s) / TENTH);
  currentState = init_st;
}

// This is a debug state print routine. It will print the names of the states
// each time tick() is called. It only prints states if they are different than
// the previous state.
void debugStatePrint() {
  static enum clockControl_st_t previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state
  // name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false; // previousState will be defined, firstPass is false.
    previousState =
        currentState;       // keep track of the last state that you were in.
    switch (currentState) { // This prints messages based upon the state that
                            // you were in.
    case init_st:
      printf(INIT_ST_MSG);
      break;
    case waiting_st:
      printf(WAITING_ST_MSG);
      break;
    case inc_dec_st:
      printf(INC_DEC_ST_MSG);
      break;
    case long_press_delay_st:
      printf(LONG_PRESS_DELAY_ST);
      break;
    case fast_update_st:
      printf(FAST_UPDATE_ST_MSG);
      break;
    }
  }
}

/*Mealy and Moore SMs. Transitions, then actions*/
void clockControl_tick() {
  debugStatePrint(); // For debugging purposes
  // Perform state update first.
  switch (currentState) {
  case init_st:
    currentState = waiting_st;
    break;
  case waiting_st:
    if (touchscreen_get_status() == TOUCHSCREEN_PRESSED) {
      currentState = long_press_delay_st;
      delay_cnt = 0;
    } else if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      currentState = inc_dec_st;
    }
    break;
  case inc_dec_st:
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      currentState = waiting_st;
      touchscreen_ack_touch();
    }
    break;
  case long_press_delay_st:
    if (delay_cnt >= delay_num_ticks) {
      currentState = fast_update_st;
      update_cnt = 0;
    } else if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      currentState = inc_dec_st;
    } else {
      currentState = long_press_delay_st;
    }
    break;
  case fast_update_st:
    // If the touchscreen is not released AND update_cnt is greater than num of
    // update ticks...
    if (touchscreen_get_status() != TOUCHSCREEN_RELEASED &&
        (update_cnt >= update_num_ticks)) {
      currentState = fast_update_st;
      update_cnt = 0;
      clockDisplay_performIncDec(touchscreen_get_location());
    } else if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      currentState = waiting_st;
      touchscreen_ack_touch();
    } else {
      currentState = fast_update_st;
    }
    break;
  default:
    printf("TRANSITION ERROR");
    break;
  }

  // Perform state action next.
  switch (currentState) {
  case init_st:
    break;
  case waiting_st:
    break;
  case inc_dec_st:
    clockDisplay_performIncDec(touchscreen_get_location());
    break;
  case long_press_delay_st:
    delay_cnt++;
    break;
  case fast_update_st:
    update_cnt++;
    break;
  default:
    printf("ACTION ERROR");
    break;
  }
}
