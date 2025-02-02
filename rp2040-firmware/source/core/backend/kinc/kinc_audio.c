#include "../../../ng_audio.h"

#include "../../../3rd/modplay/modplay.h"
#include "../../../ng_assets.h"
#include <kinc/audio1/audio.h>
#include <kinc/audio2/audio.h>
#include <kinc/math/core.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define MODBUFFER_SIZE 16383
short modBuffer[MODBUFFER_SIZE];

ModPlayerStatus_t* mod_player_status = NULL;

static void mixaudio_callback(kinc_a2_buffer_t *buffer, uint32_t samples, void* userdata){
    int write_location = buffer->write_location;
    
    kinc_a1_mix(buffer, samples);

    if (!mod_player_status){
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
    RenderMOD(modBuffer, samples);

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
void audio_update(){}
void audio_play_wav(uint8_t asset_id, bool loop){
    const uint8_t* ptr = assets_get_pointer(asset_id);
    const uint32_t size = assets_get_size(asset_id);
    kinc_a1_sound_t* sound = kinc_a1_sound_create_from_buffer(ptr,size,KINC_A1_AUDIOFORMAT_WAV);
    kinc_a1_channel_t* channel = kinc_a1_play_sound(sound,loop, 1.0f, true);
}

void audio_play_mod(uint8_t asset_id, bool loop){
    void* ptr = assets_get_pointer(asset_id);
    const uint32_t size = assets_get_size(asset_id);
    
    mod_player_status = InitMOD(ptr,44100);
    
}