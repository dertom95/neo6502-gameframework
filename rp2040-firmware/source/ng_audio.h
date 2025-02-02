#ifndef __NG_SOUND_H__
#define __NG_SOUND_H__

//#include "mod_data.h"
#include <stdint.h>
#include <stdbool.h>

void ng_audio_init(); // using prefix to avoid collision with 3d-party audio's audio_init(freq)
void audio_update();
//void audio_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop);
/*api:5:1*/void audio_play_wav(uint8_t asset_id, bool loop);
/*api:5:2*/void audio_play_mod(uint8_t asset_id, bool loop);
#endif 