#include "buttons.h"
#include "oscillator.h"
#include "sound.h"
#include "switches.h"
#include <stdint.h>
#include <stdio.h>

sound_sounds_t currSound() {}

int main() {

  writeFile();
  sound_init();
  buttons_init();
  switches_init();

  uint8_t buttonsVal = buttons_read();
  uint8_t switchesVal = switches_read();

  printf("%d\n", buttonsVal);
  printf("%d\n", switchesVal);

  sound_sounds_t currSound;

  if (switchesVal == 8) {
    currSound = sound_c4_sine_e;
  } else if (switchesVal == 12) {
    currSound = sound_c4_e4_sine_e;
  } else if (switchesVal == 14) {
    currSound = sound_c4_e4_g4_sine_e;
  } else if (switchesVal == 15) {
    currSound = sound_c4_e4_g4_c5_sine_e;
  }

  sound_tick();
  sound_playSound(currSound);

  uint8_t previousSwitchesVal;
  while (1) {
    previousSwitchesVal = switchesVal;
    if (switchesVal == 8) {
      currSound = sound_c4_sine_e;
    } else if (switchesVal == 12) {
      currSound = sound_c4_e4_sine_e;
    } else if (switchesVal == 14) {
      currSound = sound_c4_e4_g4_sine_e;
    } else if (switchesVal == 15) {
      currSound = sound_c4_e4_g4_c5_sine_e;
    }
    switchesVal = switches_read();
    sound_tick();
    if (switchesVal != previousSwitchesVal) {
      sound_stopSound();
      sound_playSound(currSound);
    }
  }
  printf("done.\n");
}