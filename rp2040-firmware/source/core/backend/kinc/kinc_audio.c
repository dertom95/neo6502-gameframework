#include "../../../ng_audio.h"

#include "../../../3rd/modplay/modplay.h"
#include "../../../ng_assets.h"
#include "../../../ng_utils.h"
#include <kinc/audio1/audio.h>
#include <kinc/audio2/audio.h>
#include <kinc/math/core.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define MODBUFFER_SIZE 16383
short modBuffer[MODBUFFER_SIZE];

typedef struct audio_state_t {
    uint8_t flags;
    uint8_t unused1;    
    uint8_t unused2;    
    uint8_t unused3;    

    ModPlayerStatus_t* mod_player_status;
} audio_state_t;

static audio_state_t audiostate;

#define audiostate_is_set(STATE) (bit_is_set_all(audiostate.flags,AUDIOSTATE_MOD_IS_PLAYING))
#define audiostate_set(STATE) (bit_set(audiostate.flags,STATE))
#define audiostate_unset(STATE) (bit_unset(audiostate.flags,STATE))
#define AUDIOSTATE_MOD_IS_PLAYING (1 << 0)

static void init_state(){
    audiostate =(audio_state_t){0};
}

static void mixaudio_callback(kinc_a2_buffer_t *buffer, uint32_t samples, void* userdata){
    int write_location = buffer->write_location;
    
    kinc_a1_mix(buffer, samples);

    if (!audiostate_is_set(AUDIOSTATE_MOD_IS_PLAYING)) {
        return;
    }

    // if (samples == 0){
    //     samples = 1024;
    //     buffer->write_location += 1024;
    //     if (buffer->write_location >= buffer->data_size){
    //         buffer->write_location -= buffer->data_size;
    //     }
    // }

    assert(MODBUFFER_SIZE>samples*2);
    assert(audiostate.mod_player_status);
    audiostate.mod_player_status = RenderMOD(modBuffer, samples);


    short* tipMod = modBuffer;
    float* tip_a1_left = buffer->channels[0] + write_location;
    float* tip_a1_right = buffer->channels[1] + write_location;
    float* left_end = buffer->channels[0] + buffer->data_size;

    for (int i=0;i<samples;i++){
        // left channel
        short mod_left = *tipMod++;
        float mod_left_float = mod_left / 32768.0f;
        float left = (*tip_a1_left + mod_left_float) / 2.0f; 
        left = kinc_clamp(left,-1.0f,1.0f);
        *tip_a1_left = left;
        tip_a1_left++;


        // right channel
        short mod_right = *tipMod++;
        float mod_right_float = mod_right / 32768.0f;
        float right = (*tip_a1_right + mod_right_float) / 2.0f;
        right = kinc_clamp(right,-1.0f,1.0f);
        *tip_a1_right = right;
        tip_a1_right++;


        if (tip_a1_left == left_end){
            tip_a1_left = buffer->channels[0];
            tip_a1_right = buffer->channels[1];
        }
    }
    assert(tip_a1_left==buffer->channels[0]+buffer->write_location);
    assert(tip_a1_right==buffer->channels[1]+buffer->write_location);
}

void ng_audio_init() {
    kinc_a1_init();
    kinc_a2_init();
    kinc_a2_set_callback(mixaudio_callback,NULL);
}

void audio_update(){
    // nothing to do yet! Magic is done in the audio-callback
}

uint8_t audio_wav_load(uint8_t asset_id){
    const uint8_t* ptr = assets_get_pointer(asset_id);
    const uint32_t size = assets_get_size(asset_id);    
    kinc_a1_sound_t* sound = kinc_a1_sound_create_from_buffer(ptr,size,KINC_A1_AUDIOFORMAT_WAV);

    uint8_t sound_id = id_store(sound);
    return sound_id;
}

void audio_wav_play(uint8_t sound_id, bool loop, bool unique){
    id_assert_validity(sound_id);
    kinc_a1_sound_t* sound = id_get_ptr(sound_id);
    kinc_a1_play_sound(sound,loop, 1.0f, unique);
}

void audio_wav_stop(uint8_t sound_id){
    id_assert_validity(sound_id);
    kinc_a1_sound_t* sound = id_get_ptr(sound_id);
    kinc_a1_stop_sound(sound);
}

void audio_mod_play(uint8_t asset_id){
    const uint8_t* ptr = assets_get_pointer(asset_id);
    const uint32_t size = assets_get_size(asset_id);
    audiostate_set(AUDIOSTATE_MOD_IS_PLAYING);
    audiostate.mod_player_status = InitMOD(ptr,44100);
}

void audio_mod_pause(){
    assert(audiostate_is_set(AUDIOSTATE_MOD_IS_PLAYING));
    audiostate_unset(AUDIOSTATE_MOD_IS_PLAYING);
}

void audio_mod_stop(){
    audio_mod_pause();
    audiostate.mod_player_status = NULL;
}

void audio_mod_resume(){
    assert(audiostate.mod_player_status != NULL);
    audiostate_set(AUDIOSTATE_MOD_IS_PLAYING);
}

uint8_t counter = 0;
uint8_t audio_mod_pos() {
    assert(audiostate.mod_player_status!=NULL);
    uint8_t pos = (uint8_t)((audiostate.mod_player_status->order*100 / audiostate.mod_player_status->orders));
    return pos;
}