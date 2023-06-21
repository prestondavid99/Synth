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
  // Init sounds and set volume
  sound_init();
  sound_setVolume(sound_mediumLowVolume_e);
  // Init buttons and switches
  buttons_init();
  switches_init();
}

// Function to update the values to control the different combination of sounds
// Right most button is C, followed by F, then G
// Right most switch is the base note, followed by a 3rd up, then another 3rd, then an octave
void update_sound_values() {
  // Read in Buttons and Switches values
  buttonsVal = buttons_read();
  switchesVal = switches_read();
  // Determine where the sounds are in the LUT
  if (buttonsVal == 1) {
    incVal = switchesVal;
  } else if (buttonsVal == 2) {
    incVal = switchesVal + 16;
  } else if (buttonsVal == 4) {
    incVal = switchesVal + 32;
  } else {
    incVal = 0;
  }
}

int main() {
  synth_init();
  while (1) {
    update_sound_values();
    sound_tick();
    // loop the current sound
    if (!sound_isBusy())
      sound_playSound(incVal);

    // check if sound has been changed since last
    if (previousSwitchesVal != switchesVal) {
      sound_stopSound();
      sound_playSound(incVal);
    }
    previousSwitchesVal = switchesVal;
  }
}