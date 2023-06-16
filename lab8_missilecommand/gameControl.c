#include "gameControl.h"
#include "config.h"
#include "display.h"
#include "interrupts.h"
#include "intervalTimer.h"
#include "missile.h"
#include "plane.h"
#include "touchscreen.h"
#include <math.h>
#include <stdio.h>

#define SQUARED 2
#define BOTTOM_TICKS (CONFIG_MAX_TOTAL_MISSILES / 2)
#define TEXT_SIZE 1
#define HALF_DISPLAY (DISPLAY_WIDTH / 2)

missile_t missiles[CONFIG_MAX_TOTAL_MISSILES];
missile_t *enemy_missiles = &(missiles[0]);
missile_t *player_missiles = &(missiles[CONFIG_MAX_ENEMY_MISSILES]);
missile_t *plane_missile = &(missiles[CONFIG_MAX_PLANE_MISSILES]);
uint16_t impactedCount;
uint16_t shotCount;
bool halfTicks;

bool isWithinRadius(missile_t *enemy, missile_t *missile) {
  return (pow(enemy->x_current - missile->x_current, SQUARED) +
          pow(enemy->y_current - missile->y_current, SQUARED)) <
         pow(missile->radius, SQUARED);
}

bool isWithinRadiusPlane(uint16_t x, uint16_t y, missile_t *missile) {
  return (pow(x - missile->x_current, SQUARED) +
          pow(y - missile->y_current, SQUARED)) < pow(missile->radius, SQUARED);
}

void drawText(int16_t x, int16_t y, char str[], uint32_t score) {
  display_setCursor(x, y);
  display_setTextColorBg(DISPLAY_WHITE, DISPLAY_BLACK);
  display_setTextSize(TEXT_SIZE);
  display_print(str);
  display_printDecimalInt(score);
}

void gameControl_init() {
  display_init();
  display_fillScreen(DISPLAY_BLACK);
  drawText(40, 5, "Shot: ", shotCount);
  drawText(160, 5, "Impacted: ", impactedCount);
#ifdef LAB8_M3
  plane_init(&plane_missile[0]);
#endif
  shotCount = 0;
  impactedCount = 0;

  // Check for dead enemy missiles and re-initialize
  for (uint16_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++)
    if (missile_is_dead(&enemy_missiles[i])) {
      missile_init_dead(&enemy_missiles[i]);
      missile_init_enemy(&enemy_missiles[i]);
    }

  // Check for dead player missiles and re-initialize
  for (uint16_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++)
    if (missile_is_dead(&player_missiles[i])) {
      missile_init_dead(&player_missiles[i]);
      missile_init_player(&player_missiles[i], 0, 0);
    }
}

void gameControl_tick() {
#ifdef LAB8_M3
  plane_tick();
#endif

  for (uint16_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++)
    if (missile_is_dead(&enemy_missiles[i])) {
      missile_init_enemy(&enemy_missiles[i]);
    }

  for (uint16_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++) {
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      if ((missile_is_dead(&player_missiles[i]))) {
        missile_init_player(&player_missiles[i], touchscreen_get_location().x,
                            touchscreen_get_location().y);
        shotCount++;
        touchscreen_ack_touch();
        break;
      }
    }
  }

#ifdef LAB8_M3
  if (plane_getXY().x >= DISPLAY_WIDTH)
    missile_init_plane(&plane_missile[0], plane_getXY().x, plane_getXY().y);
#endif

  // both halves of missiles
  if (halfTicks) {
    // tick the first half
    for (uint16_t i = 0; i < BOTTOM_TICKS; i++) {
      missile_tick(&missiles[i]);
      if (missiles[i].impacted == true)
        impactedCount++;
      drawText(40, 5, "Shot: ", shotCount);
      drawText(160, 5, "Impacted: ", impactedCount);
    }
    halfTicks = !halfTicks;
    // Tick the other half
  } else {
    // Tick the other half
    for (uint16_t i = BOTTOM_TICKS; i < CONFIG_MAX_TOTAL_MISSILES; i++) {
      missile_tick(&missiles[i]);
      if (missiles[i].impacted == true)
        impactedCount++;
      drawText(40, 5, "Shot: ", shotCount);
      drawText(160, 5, "Impacted: ", impactedCount);
    }
    halfTicks = !halfTicks;
  }

  // calculate the missile collisions
  for (uint16_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++) {
    // calculate the missile collisions
    for (uint16_t j = 0; j < CONFIG_MAX_TOTAL_MISSILES; j++) {
      if (!(missile_is_flying(&missiles[i])))
        continue;
      if (!(missile_is_exploding(&missiles[j])))
        continue;
      if (isWithinRadius(&missiles[i], &missiles[j]))
        missile_trigger_explosion(&missiles[i]);
#ifdef LAB8_M3
      if (isWithinRadiusPlane(plane_getXY().x, plane_getXY().y, &missiles[j])) {

        plane_explode();
      }
#endif
    }
  }
} // NO
