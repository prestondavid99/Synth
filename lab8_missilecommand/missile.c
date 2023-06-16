#include "missile.h"
#include "config.h"
#include "display.h"
#include "touchscreen.h"
#include "xparameters.h"
#include <math.h>
#include <stdio.h>

#define MISSILE_SPAWN (DISPLAY_HEIGHT / 6)
#define MISSILE_SPAWN_OFFSET 10
#define LAUNCH_SITE_1 (DISPLAY_WIDTH / 4)
#define LAUNCH_SITE_2 (LAUNCH_SITE_1 * 2)
#define LAUNCH_SITE_3 (LAUNCH_SITE_2 + LAUNCH_SITE_1)
#define LEFT_DIVIDER (DISPLAY_WIDTH / 3)
#define RIGHT_DIVIDER (LEFT_DIVIDER * 2)
#define QUAD 4

// SM States
enum missle_st {
  init_st,
  moving_st,
  explode_grow_st,
  explode_shrink_st,
  dead_st
};

// This will randomize an X value
uint16_t xRandomizer() { return rand() % DISPLAY_WIDTH; }

// This will calculate the length of the missile using the equation give to us
// in the slides
double calcLength(missile_t *missile) {
  uint16_t x1 = missile->x_origin;
  uint16_t y1 = missile->y_origin;
  uint16_t x2 = missile->x_dest;
  uint16_t y2 = missile->y_dest;
  return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

// This will calculate the current position using the equation given to us in
// the slides
int16_t calcCurrentPos(uint16_t origin, double percentage, uint16_t dest) {
  return origin + percentage * (dest - origin);
}

// This will increment the missle's trail
void update_missile(missile_t *missile, double distancePerTick,
                    uint16_t color) {
  missile->length += distancePerTick * QUAD;
  double percentage = missile->length / missile->total_length;
  // printf("%f\n", percentage);
  missile->x_current =
      calcCurrentPos(missile->x_origin, percentage, missile->x_dest);

  missile->y_current =
      calcCurrentPos(missile->y_origin, percentage, missile->y_dest);
  display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                   missile->y_current, color);
}

// This is a helper function that will draw the explosion circle
void drawCircle(missile_t *missile, uint16_t color) {
  display_fillCircle(missile->x_current, missile->y_current, missile->radius,
                     color);
}

// This will choose between the types of explosions to draw the correct
// explosion color
void drawCircleType(missile_t *missile) {
  if (missile->type == MISSILE_TYPE_ENEMY) {
    display_fillCircle(missile->x_current, missile->y_current, missile->radius,
                       DISPLAY_RED);
  } else if (missile->type == MISSILE_TYPE_PLAYER) {
    display_fillCircle(missile->x_current, missile->y_current, missile->radius,
                       DISPLAY_GREEN);
  } else {
    display_fillCircle(missile->x_current, missile->y_current, missile->radius,
                       DISPLAY_BLUE);
  }
}

// This will initialize a dead missile
void missile_init_dead(missile_t *missile) {
  missile->currentState = dead_st;
  missile->explode_me = false;
}

// This will initialize an enemy missile
void missile_init_enemy(missile_t *missile) {
  missile->type = MISSILE_TYPE_ENEMY;

  missile->x_origin = rand() % DISPLAY_WIDTH;

  // This is so there is room for the text
  missile->y_origin = (rand() % MISSILE_SPAWN) + MISSILE_SPAWN_OFFSET;

  missile->x_dest = rand() % DISPLAY_WIDTH;
  missile->y_dest = DISPLAY_HEIGHT;

  missile->currentState = init_st;
}

// This will initialize a player missile
void missile_init_player(missile_t *missile, uint16_t x_dest, uint16_t y_dest) {
  missile->type = MISSILE_TYPE_PLAYER;
  missile->y_origin = DISPLAY_HEIGHT;
  missile->y_dest = y_dest;
  missile->x_dest = x_dest;
  missile->currentState = init_st;

  if (x_dest <= LEFT_DIVIDER) {
    missile->x_origin = LAUNCH_SITE_1;
  } else if ((x_dest > LEFT_DIVIDER) && (x_dest <= RIGHT_DIVIDER)) {
    missile->x_origin = LAUNCH_SITE_2;
  } else if (x_dest > RIGHT_DIVIDER) {
    missile->x_origin = LAUNCH_SITE_3;
  }
}

// This will initialize the missile of the plane.
void missile_init_plane(missile_t *missile, int16_t plane_x, int16_t plane_y) {
  missile->type = MISSILE_TYPE_PLANE;

  missile->x_origin = plane_x;
  missile->y_origin = plane_y;

  missile->y_dest = DISPLAY_HEIGHT;
  missile->x_dest = xRandomizer();

  missile->currentState = init_st;
}

// This is the SM that will tick the missiles
void missile_tick(missile_t *missile) {
  /*MEALY*/
  switch (missile->currentState) {
  case init_st:
    missile->length = 0;
    missile->explode_me = false;
    missile->total_length = calcLength(missile);
    missile->x_current = missile->x_origin;
    missile->y_current = missile->y_origin;
    missile->impacted = false;
    missile->currentState = moving_st;
    break;
  case moving_st:
    // if missile exploded, erase it, and enter explode state.
    if (missile->explode_me == true) {
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, DISPLAY_BLACK);
      missile->currentState = explode_grow_st;
      missile->explode_me = true;
    }
    if (missile->length >= missile->total_length) {
      // if it's a player missile, explode it
      if (missile->type == MISSILE_TYPE_PLAYER) {
        display_drawLine(missile->x_origin, missile->y_origin,
                         missile->x_current, missile->y_current, DISPLAY_BLACK);
        missile->currentState = explode_grow_st;
        missile->explode_me = true;
        // if it's any other missile, send it to dead because it made it to
        // impact
      } else {
        display_drawLine(missile->x_origin, missile->y_origin,
                         missile->x_current, missile->y_current, DISPLAY_BLACK);
        missile->currentState = dead_st;
        missile->impacted = true;
      }
    }
    break;
  case explode_grow_st:

    if (missile->radius >= CONFIG_EXPLOSION_MAX_RADIUS) {
      missile->currentState = explode_shrink_st;
    }

    break;
  case explode_shrink_st:
    // printf("%s\n", "SHRINK_ST");
    if (missile->radius <= 0) {
      missile->currentState = dead_st;
    }
    break;
  case dead_st:
    missile->explode_me = false;
    break;
  default:
    printf("%s\n", "Error!");
    break;
  }

  /*MOORE*/
  switch (missile->currentState) {
  case init_st:

    break;
  case moving_st:
    if (missile->type == MISSILE_TYPE_PLAYER) {
      // erase previous line and update missile for the player
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, DISPLAY_BLACK);
      update_missile(missile, CONFIG_PLAYER_MISSILE_DISTANCE_PER_TICK,
                     DISPLAY_GREEN);
      // erase previous line and update the missile line for the enemy.
    } else if (missile->type == MISSILE_TYPE_ENEMY) {
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, DISPLAY_BLACK);
      update_missile(missile, CONFIG_ENEMY_MISSILE_DISTANCE_PER_TICK,
                     DISPLAY_RED);
      // erase previous line and update the missile line for plane missile
    } else {
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, DISPLAY_BLACK);
      update_missile(missile, CONFIG_ENEMY_MISSILE_DISTANCE_PER_TICK,
                     DISPLAY_BLUE);
    }
    break;
  case explode_grow_st:
    missile->radius += CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK * QUAD;
    drawCircleType(missile);
    break;
  case explode_shrink_st:
    drawCircle(missile, DISPLAY_BLACK);
    missile->radius -= CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK * QUAD;
    drawCircleType(missile);
    break;
  case dead_st:
    break;
  default:
    break;
  }
}

// return true if the missile is dead
bool missile_is_dead(missile_t *missile) {
  return (missile->currentState == dead_st);
}

// return true if the missile is exploding
bool missile_is_exploding(missile_t *missile) {
  return (missile->explode_me == true);
}

// return true if the missile is moving
bool missile_is_flying(missile_t *missile) {
  return (missile->currentState == moving_st);
}

// return true if the missile is exploding
void missile_trigger_explosion(missile_t *missile) {
  missile->explode_me = true;
}
