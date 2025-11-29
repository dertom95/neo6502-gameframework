#ifndef __NG_SOUND_H__
#define __NG_SOUND_H__

//#include "mod_data.h"
#include <stdint.h>
#include <stdbool.h>

#include "ng_utils.h"

typedef struct audio_state_t {
    uint8_t flags;
} audio_state_t;

extern audio_state_t audiostate;

#define audiostate_is_set(STATE) (bit_is_set_all(audiostate.flags,AUDIOSTATE_MOD_IS_PLAYING))
#define audiostate_set(STATE) (bit_set(audiostate.flags,STATE))
#define audiostate_unset(STATE) (bit_unset(audiostate.flags,STATE))

#define AUDIOSTATE_MOD_IS_PLAYING (1 << 0)

void ng_audio_init(); // using prefix to avoid collision with 3d-party audio's audio_init(freq)
void audio_update();
//void audio_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop);
/*api:5:5*/uint8_t audio_wav_load(uint8_t asset_id);
/*api:5:1*/void audio_wav_play(uint8_t sound_id, bool loop);
/*api:5:4*/void audio_wav_stop(uint8_t sound_id);

/*api:5:2*/void audio_mod_play(uint8_t asset_id);
/*api:5:7*/void audio_mod_pause(void);
/*api:5:8*/void audio_mod_resume(void);
/*api:5:3*/void audio_mod_stop();
/*api:5:6*/uint8_t audio_mod_pos();

// PLEASE: ALWAYS MAINTAIN: LAST API ID 5:8
#endif 