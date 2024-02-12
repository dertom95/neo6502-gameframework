#if 0
#include "ng_sound.h"

#include <stdio.h>
#include <pico/stdlib.h>
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "audio.h"

#include "ng_gfx.h"

#include "mod_data.h"
#include "mod_play.h"

#define SOUND_PIN  20  // sound output

// Call the MOD player to fill the output audio buffer.
// This must be called every 20 miliseconds or so, or more
// often if SOUND_OUTPUT_FREQUENCY is increased.
static void update_mod_player(void)
{
  uint8_t *audio_buffer = audio_get_buffer();
  if (audio_buffer) {
    mod_play_step(audio_buffer, AUDIO_BUFFER_SIZE);
  }
}

void sound_init(int freq)
{
  bi_decl(bi_program_description("MOD test"));
  bi_decl(bi_1pin_with_name(SOUND_PIN, "Sound output"));
  
  audio_init(SOUND_PIN, freq);
}

void sound_play_mod(const struct MOD_DATA* mod_data,int frequency, bool loop)
{
  //mod_play_start(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY, 1);
  mod_play_start(mod_data, frequency, loop ? 1 : 0);
}

void sound_update()
{
  update_mod_player();
  audio_mixer_step();
}
#endif
