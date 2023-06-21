#include "buttons.h"
#include "oscillator.h"
#include "sound.h"
#include "sounds/C4_sine.wav.h"
#include "switches.h"
#include <stdint.h>
#include <stdio.h>

sound_sounds_t currSound() {}

int main() {
  sound_init();
  switches_init();
  sound_setVolume(sound_mediumHighVolume_e);

  while (1) {
    sound_tick();
    uint8_t switchesVal = switches_read();
    printf("%d\n", switchesVal);
    if (!sound_isBusy()) {
      sound_playSound(switchesVal);
    }
  }
}