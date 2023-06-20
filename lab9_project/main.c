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
  buttons_init();
  switches_init();

  sound_setVolume(sound_mediumHighVolume_e);

  uint8_t buttonsVal = buttons_read();
  uint8_t switchesVal = switches_read();

  printf("%d\n", switchesVal);
  uint16_t **LUT = getSoundsLUT();
  printf("%d\n", LUT[switchesVal][2]);

  sound_tick();
  sound_playSound(switchesVal);

  while (1) {
    switchesVal = switches_read();
    sound_tick();
    if (!sound_isBusy) {
      sound_stopSound();
      sound_playSound(switchesVal);
    }
  }
  printf("done.\n");
}