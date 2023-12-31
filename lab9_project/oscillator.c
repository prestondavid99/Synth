#include <math.h>
#include <stdio.h>
#define PI 3.14159
#define SAMPLE_RATE 44100

float sineOscillator(float freq, float amp) {
  return amp * sin((2 * PI * freq) / SAMPLE_RATE);

  // amp*sin(2*pi*freq/samplingRate)
}

void writeFile() {
  FILE *fptr;
  fptr = fopen("soundOutput.c", "w");
  if (fptr == NULL) {
    printf("Error opening file\n");
  } else {
    printf("Writing File...\n");
  }

  fputs("#include <stdint.h> \n\n", fptr);
  fclose(fptr);
}