add_library(sounds

gameBoyStartup.wav.c

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
)

target_link_libraries(sounds ${330_LIBS})
