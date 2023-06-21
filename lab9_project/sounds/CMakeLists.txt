add_library(sounds

C4_sine.wav.c
E4_sine.wav.c
C4_E4_sine.wav.c
G4_sine.wav.c
C4_G4_sine.wav.c
E4_G4_sine.wav.c
C4_E4_G4_sine.wav.c
C5_sine.wav.c
C4_C5_sine.wav.c
E4_C5_sine.wav.c
C4_E4_C5_sine.wav.c
G4_C5_sine.wav.c
C4_G4_C5_sine.wav.c
E4_G4_C5_sine.wav.c
C4_E4_G4_C5_sine.wav.c

F4_sine.wav.c
A4_sine.wav.c
F4_A4_sine.wav.c
C4_sine.wav.c
F4_C4_sine.wav.c
A4_C4_sine.wav.c
F4_A4_C4_sine.wav.c
F5_sine.wav.c
F4_F5_sine.wav.c
A4_F5_sine.wav.c
F4_A4_F5_sine.wav.c
C4_F5_sine.wav.c
F4_C4_F5_sine.wav.c
A4_C4_F5_sine.wav.c
F4_A4_C4_F5_sine.wav.c

G4_sine.wav.c
B4_sine.wav.c
G4_B4_sine.wav.c
D4_sine.wav.c
G4_D4_sine.wav.c
B4_D4_sine.wav.c
G4_B4_D4_sine.wav.c
G5_sine.wav.c
G4_G5_sine.wav.c
B4_G5_sine.wav.c
G4_B4_G5_sine.wav.c
D4_G5_sine.wav.c
G4_D4_G5_sine.wav.c
B4_D4_G5_sine.wav.c
G4_B4_D4_G5_sine.wav.c
)

target_link_libraries(sounds ${330_LIBS})
