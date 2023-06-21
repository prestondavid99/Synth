#include "buttons.h"
#include "oscillator.h"
#include "sound.h"
#include "sounds/C4_sine.wav.h"
#include "switches.h"
#include <stdint.h>
#include <stdio.h>

uint8_t previousSwitchesVal, incVal, switchesVal, buttonsVal;

// Initialization function for the Lab 9 Synth
void synth_init() {
  sound_init();
  sound_setVolume(sound_mediumLowVolume_e);

  buttons_init();
  switches_init();
}

int main() {
  synth_init();
  while (1) {
    buttonsVal = buttons_read();
    switchesVal = switches_read() + buttons_read();
    sound_tick();
    // loop the current sound
    if (!sound_isBusy())
      sound_playSound(switchesVal);

    // check if sound has been changed since last
    if (previousSwitchesVal != switchesVal) {
      sound_stopSound();
      sound_playSound(switchesVal);
    }

    previousSwitchesVal = switchesVal;
  }
}