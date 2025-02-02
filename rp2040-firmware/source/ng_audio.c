#include "ng_audio.h"

#include <assert.h>

audio_state_t audiostate;

extern void ng_audio_init_backend(void);

void ng_audio_init(){
    audiostate =(audio_state_t){0};    
    ng_audio_init_backend();
}

void audio_mod_pause(){
    assert(audiostate_is_set(AUDIOSTATE_MOD_IS_PLAYING));
    audiostate_unset(AUDIOSTATE_MOD_IS_PLAYING);
}

void audio_mod_resume(){
    audiostate_set(AUDIOSTATE_MOD_IS_PLAYING);
}