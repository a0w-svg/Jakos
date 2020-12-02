#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>
#include "../../common/include/port.h"

void play_sound(uint32_t num_frequence);
void no_sound();
void beep();
#endif