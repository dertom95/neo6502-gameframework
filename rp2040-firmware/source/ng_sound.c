#include "ng_sound.h"

#ifdef PICO_NEO6502

#include <stdio.h>
#include <pico/stdlib.h>
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "3rd/audio/audio.h"

#include "ng_gfx.h"
#include "ng_assets.h"

#include "mod_data.h"
#include "mod_play.h"

#define SOUND_PIN  20  // sound output

void init_tsf();
void update_tsf();

// Call the MOD player to fill the output audio buffer.
// This must be called every 20 miliseconds or so, or more
// often if SOUND_OUTPUT_FREQUENCY is increased.
static void update_mod_player(uint8_t * audio_buffer)
{
  if (audio_buffer) {
    mod_play_step(audio_buffer, AUDIO_BUFFER_SIZE);
  }
}

void sound_init(int freq)
{
  bi_decl(bi_program_description("MOD test"));
  bi_decl(bi_1pin_with_name(SOUND_PIN, "Sound output"));
  
  audio_init(SOUND_PIN, freq);

 
  
  //init_tsf();
}

void sound_play_wav(uint8_t asset_id, bool loop) {
  const void* music_ptr = assets_get_pointer(asset_id);
  const uint32_t music_size = assets_get_size(asset_id);
  int source_music = loop 
                        ? audio_play_loop(music_ptr,music_size,0)
                        : audio_play_once(music_ptr,music_size);
}

void sound_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop)
{
  //mod_play_start(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY, 1);
  mod_play_start(mod_data, frequency, loop ? 1 : 0);
}

bool mod_playing = true;

void sound_update()
{
  uint8_t *audio_buffer = audio_get_buffer();
  if (audio_buffer){
    update_mod_player(audio_buffer);
    //update_tsf();
    audio_mixer_step(audio_buffer, mod_playing);
  }

}
#else
#include <stdbool.h>
#include <stdint.h>
void sound_init(int frequency) {}
void sound_update(){}
void sound_play_wav(uint8_t asset_id, bool loop){}
#endif
