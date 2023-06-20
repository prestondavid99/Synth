#include "buttons.h"
#include "sound.h"
#include <stdio.h>

int main() {
  sound_init();
  sound_tick();
  sound_playSound(sound_cMidi_e);
  
  while (1) {
    sound_tick();
    if (!sound_isBusy())
      break;
  }
  printf("done.\n");
}