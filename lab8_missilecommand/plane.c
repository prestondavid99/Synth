#include "plane.h"
#include "config.h"
#include "missile.h"
#include <stdio.h>

#define X0 (x - 25)
#define YREF_1 DISPLAY_HEIGHT - 200
#define YREF_2 DISPLAY_HEIGHT - 210
#define YREF_3 DISPLAY_HEIGHT - 190

// States for plane_tick SM
enum plane_st { init_st, moving_st, explode_st, dead_st };
static enum plane_st currentState;
uint16_t tickCount;
uint16_t planeX;
uint16_t planeY;

// helper function to draw plane
void draw_plane(uint16_t x, uint16_t color) {
  display_fillTriangle(X0, YREF_1, x, YREF_2, x, YREF_3, color);
}

// Move plane per tick.
void move_plane(uint16_t x) {
  planeX = x - CONFIG_PLANE_DISTANCE_PER_TICK;
  draw_plane(x, DISPLAY_BLACK);
  draw_plane(planeX, DISPLAY_WHITE);
}

// initialize the plane.
void plane_init(missile_t *plane_missile) {
  planeX = DISPLAY_WIDTH + 25;
  planeY = YREF_1;
}

// SM for the plane
void plane_tick() {
  // MEALY
  switch (currentState) {
  case init_st:
    currentState = moving_st;
    break;
  case moving_st:
    if (planeX <= 0) {
      currentState = dead_st;
    }
    break;

  case explode_st:
    break;

  case dead_st:

    if (tickCount >= 200) {
      currentState = init_st;
    }
    break;
  default:
    break;
  }

  // MOORE
  switch (currentState) {
  case init_st:
    tickCount = 0;
    break;
  case moving_st:
    move_plane(planeX);
    break;

  case explode_st:
    break;

  case dead_st:
    tickCount++;
    break;
  default:
    break;
  }
}

// Erase the plane
void plane_explode() {
  currentState = dead_st;
  draw_plane(planeX, DISPLAY_BLACK);
  planeX = DISPLAY_WIDTH + 25;
  planeY = YREF_1;
}

// Get xy coordinate of plane
display_point_t plane_getXY() {
  display_point_t plane;
  plane.x = planeX;
  plane.y = planeY;
  return plane;
}
