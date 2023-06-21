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
  uint8_t previousSwitchesVal, switchesVal = switches_read();

  sound_tick();
  sound_setSound(switchesVal);
  sound_startSound();
  while (1) {
    switchesVal = switches_read();
    sound_tick();
    if (previousSwitchesVal != switchesVal) {
      sound_stopSound();
      sound_playSound(switchesVal);
    }
    if (!sound_isBusy()) {
      sound_playSound(switchesVal);
    }
    previousSwitchesVal = switchesVal;
  }
}