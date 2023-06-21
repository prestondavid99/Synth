#include "buttons.h"
#include "oscillator.h"
#include "sound.h"
#include "sounds/C4_sine.wav.h"
#include "switches.h"
#include <stdint.h>
#include <stdio.h>

int main() {

  sound_init();
  buttons_init();
  switches_init();

  sound_setVolume(sound_mediumHighVolume_e);

  uint8_t buttonsVal = buttons_read();
  uint8_t switchesVal = switches_read();


  sound_tick();
  sound_setSound(switchesVal);
  sound_startSound();
  while (1) {
    sound_tick();
    if (!sound_isBusy())
      break;
  }
}