#include "buttons.h"
#include "oscillator.h"
#include "sound.h"
#include "switches.h"
#include <stdint.h>
#include <stdio.h>

int main() {
  sound_init();
  buttons_init();
  switches_init();

  uint8_t buttonsVal = buttons_read();
  uint8_t switchesVal = switches_read();

  printf("%d\n", buttonsVal);
  printf("%d\n", switchesVal);

  sound_tick();
  sound_playSound(sound_c4_sine_e);

  while (1) {
    sound_tick();
    if (!sound_isBusy())
      sound_playSound(sound_c4_sine_e);
  }
  printf("done.\n");
}