#if 0
#ifndef __NG_SOUND_H__
#define __NG_SOUND_H__

#include "mod_data.h"
#include "stdbool.h"


#define SOUND_OUTPUT_FREQUENCY_11K 11025 
#define SOUND_OUTPUT_FREQUENCY_22K 22050 // frequency of the output wave (samples/second)

void sound_init(int frequency);
void sound_update();
void sound_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop);

#endif 
#endif