add_executable(lab9.elf main.c sound.c timer_ps.c)
add_subdirectory(sounds)
target_link_libraries(lab9.elf ${330_LIBS} intervalTimer touchscreen interrupts sounds)
set_target_properties(lab9.elf PROPERTIES LINKER_LANGUAGE CXX)