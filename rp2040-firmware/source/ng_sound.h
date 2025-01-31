#ifndef __NG_SOUND_H__
#define __NG_SOUND_H__

//#include "mod_data.h"
#include <stdint.h>
#include <stdbool.h>
#include "3rd/audio/mod_data.h"

void sound_init(int frequency);
void sound_update();
void sound_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop);
/*api:5:1*/void sound_play_wav(uint8_t asset_id, bool loop);
#endif 