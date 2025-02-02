#include <string.h>
#include <stdio.h>

#include "../../ng_defines.h"

#include "audio.h"

uint32_t single_sample = 0;
uint32_t *single_sample_ptr = &single_sample;


uint8_t audio_buffers[2][AUDIO_BUFFER_SIZE];
volatile int cur_audio_buffer;
static volatile int last_audio_buffer;

struct MIXER_SOURCE {
  const unsigned char *samples;
  int len;
  int loop_start;
  int pos;
  bool active;
  bool loop;
  uint16_t volume; // 8.8 fixed point
};

static struct MIXER_SOURCE mixer_sources[AUDIO_MAX_SOURCES];
static int16_t mixer_buffer[AUDIO_BUFFER_SIZE];

int16_t* audio_get_mixerbuffer(void){
    return mixer_buffer;
}

uint8_t *audio_get_buffer(void)
{
  if (last_audio_buffer == cur_audio_buffer) {
    return NULL;
  }

  uint8_t *buf = audio_buffers[last_audio_buffer];
  last_audio_buffer = cur_audio_buffer;
  return buf;
}

static int audio_claim_unused_source(void)
{
  for (int i = 0; i < AUDIO_MAX_SOURCES; i++) {
    if (! mixer_sources[i].active) {
      mixer_sources[i].active = true;
      return i;
    }
  }
  return -1;
}

int audio_play_once(const uint8_t *samples, int len)
{
  int source_id = audio_claim_unused_source();
  if (source_id < 0) {
    return -1;
  }
  struct MIXER_SOURCE *source = &mixer_sources[source_id];
  source->samples = samples;
  source->len = len;
  source->pos = 0;
  source->loop = false;
  source->volume = 256;
  return source_id;
}

int audio_play_loop(const uint8_t *samples, int len, int loop_start)
{
  int source_id = audio_play_once(samples, len);
  if (source_id < 0) {
    return -1;
  }
  struct MIXER_SOURCE *source = &mixer_sources[source_id];
  source->loop = true;
  source->loop_start = loop_start;
  return source_id;
}

void audio_source_stop(int source_id)
{
  mixer_sources[source_id].active = false;
}

void audio_source_set_volume(int source_id, uint16_t volume)
{
  mixer_sources[source_id].volume = volume;
}

void audio_mixer_step(uint8_t *audio_buffer, bool mod_playing)
{
  if (! audio_buffer) return;

  // setup 16-bit mixer buffer
  if (mod_playing){
    for (int i=0;i<AUDIO_BUFFER_SIZE;i++){
        mixer_buffer[i] = ((audio_buffer[i]-128) * 256) >> 8;
    }
  }
  else {
    memset(mixer_buffer, 0, sizeof(mixer_buffer));
  }


  // mix to 16-bit mixer buffer
  for (int i = 0; i < AUDIO_MAX_SOURCES; i++) {
    if (! mixer_sources[i].active) continue;
    struct MIXER_SOURCE *source = &mixer_sources[i];

    // mix source
    int mix_len = source->len - source->pos;
    if (mix_len > AUDIO_BUFFER_SIZE) {
      mix_len = AUDIO_BUFFER_SIZE;
    }
    for (int i = 0; i < mix_len; i++) {
      mixer_buffer[i] += ((source->samples[i+source->pos]-128) * source->volume) >> 8;
    }
    source->pos += mix_len;

    // handle source termination
    if (source->pos == source->len) {
      if (source->loop) {
        source->pos = source->loop_start;
      } else {
        source->active = false;
      }
    }
  }

  // convert 16-bit mixer buffer to 8-bit output buffer
  for (int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
    int sample = mixer_buffer[i] + 128;
    if (sample < 0) {
      audio_buffer[i] = 0;
    } else if (sample > 255) {
      audio_buffer[i] = 255;
    } else {
      audio_buffer[i] = sample;
    }
  }
}
