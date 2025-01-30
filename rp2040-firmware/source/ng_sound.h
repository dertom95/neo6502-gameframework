#ifndef __NG_SOUND_H__
#define __NG_SOUND_H__

#include "mod_data.h"

#include <stdbool.h>

void sound_init(int frequency);
void sound_update();
void sound_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop);

#endif 