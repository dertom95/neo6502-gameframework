#include "../../../3rd/audio/audio.h"
#include "../../../ng_audio.h"

#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/sync.h"
#include "hardware/clocks.h"

#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "../../../3rd/audio/audio.h"
#include "../../../3rd/modplay/modplay.h"

#include "../../../ng_gfx.h"
#include "../../../ng_assets.h"

#define SAMPLERATE 11025

#define AUDIO_PIN 20

static int pwm_dma_chan, trigger_dma_chan, sample_dma_chan;

extern uint8_t audio_buffers[2][AUDIO_BUFFER_SIZE];
extern volatile int cur_audio_buffer;
extern uint32_t *single_sample_ptr;
extern uint32_t single_sample;

static void __isr __time_critical_func(dma_handler)()
{
  cur_audio_buffer = 1 - cur_audio_buffer;
  dma_hw->ch[sample_dma_chan].al1_read_addr       = (intptr_t) &audio_buffers[cur_audio_buffer][0];
  dma_hw->ch[trigger_dma_chan].al3_read_addr_trig = (intptr_t) &single_sample_ptr;

  dma_hw->ints1 = 1u << trigger_dma_chan;
}

void ng_audio_init()
{
  gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);

  int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
  int audio_pin_chan = pwm_gpio_to_channel(AUDIO_PIN);

  uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
  float clock_div = ((float)f_clk_sys * 1000.0f) / 254.0f / (float) SAMPLERATE / (float) REPETITION_RATE;

  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, clock_div);
  pwm_config_set_wrap(&config, 254);
  pwm_init(audio_pin_slice, &config, true);

  pwm_dma_chan     = dma_claim_unused_channel(true);
  trigger_dma_chan = dma_claim_unused_channel(true);
  sample_dma_chan  = dma_claim_unused_channel(true);

  // setup PWM DMA channel
  dma_channel_config pwm_dma_chan_config = dma_channel_get_default_config(pwm_dma_chan);
  channel_config_set_transfer_data_size(&pwm_dma_chan_config, DMA_SIZE_32);              // transfer 32 bits at a time
  channel_config_set_read_increment(&pwm_dma_chan_config, false);                        // always read from the same address
  channel_config_set_write_increment(&pwm_dma_chan_config, false);                       // always write to the same address
  channel_config_set_chain_to(&pwm_dma_chan_config, sample_dma_chan);                    // trigger sample DMA channel when done
  channel_config_set_dreq(&pwm_dma_chan_config, DREQ_PWM_WRAP0 + audio_pin_slice);       // transfer on PWM cycle end
  dma_channel_configure(pwm_dma_chan,
                        &pwm_dma_chan_config,
                        &pwm_hw->slice[audio_pin_slice].cc,   // write to PWM slice CC register
                        &single_sample,                       // read from single_sample
                        REPETITION_RATE,                      // transfer once per desired sample repetition
                        false                                 // don't start yet
                        );

  // setup trigger DMA channel
  dma_channel_config trigger_dma_chan_config = dma_channel_get_default_config(trigger_dma_chan);
  channel_config_set_transfer_data_size(&trigger_dma_chan_config, DMA_SIZE_32);          // transfer 32-bits at a time
  channel_config_set_read_increment(&trigger_dma_chan_config, false);                    // always read from the same address
  channel_config_set_write_increment(&trigger_dma_chan_config, false);                   // always write to the same address
  channel_config_set_dreq(&trigger_dma_chan_config, DREQ_PWM_WRAP0 + audio_pin_slice);   // transfer on PWM cycle end
  dma_channel_configure(trigger_dma_chan,
                        &trigger_dma_chan_config,
                        &dma_hw->ch[pwm_dma_chan].al3_read_addr_trig,     // write to PWM DMA channel read address trigger
                        &single_sample_ptr,                               // read from location containing the address of single_sample
                        REPETITION_RATE * AUDIO_BUFFER_SIZE,              // trigger once per audio sample per repetition rate
                        false                                             // don't start yet
                        );
  dma_channel_set_irq1_enabled(trigger_dma_chan, true);    // fire interrupt when trigger DMA channel is done
  irq_set_exclusive_handler(DMA_IRQ_1, dma_handler);
  irq_set_enabled(DMA_IRQ_1, true);

  // setup sample DMA channel
  dma_channel_config sample_dma_chan_config = dma_channel_get_default_config(sample_dma_chan);
  channel_config_set_transfer_data_size(&sample_dma_chan_config, DMA_SIZE_8);  // transfer 8-bits at a time
  channel_config_set_read_increment(&sample_dma_chan_config, true);            // increment read address to go through audio buffer
  channel_config_set_write_increment(&sample_dma_chan_config, false);          // always write to the same address
  dma_channel_configure(sample_dma_chan,
                        &sample_dma_chan_config,
                        (char*)&single_sample + 2*audio_pin_chan,  // write to single_sample
                        &audio_buffers[0][0],                      // read from audio buffer
                        1,                                         // only do one transfer (once per PWM DMA completion due to chaining)
                        false                                      // don't start yet
                        );


  // clear audio buffers
  memset(audio_buffers[0], 128, AUDIO_BUFFER_SIZE);
  memset(audio_buffers[1], 128, AUDIO_BUFFER_SIZE);
  
  // kick things off with the trigger DMA channel
  dma_channel_start(trigger_dma_chan);
}

#define MODBUFFER_SIZE (AUDIO_BUFFER_SIZE*2)
short modBuffer[MODBUFFER_SIZE];

ModPlayerStatus_t* mod_player_status = NULL;
bool mod_loop = false;
#define SOUND_PIN  20  // sound output

void init_tsf();
void update_tsf();

uint8_t combine_to_mono(int16_t left, int16_t right) {
    // Step 1: Average the left and right channels
    int32_t mono = (left + right) / 2;

    // Step 2: Normalize to uint8_t range (0 to 255)
    // Shift the range from [-32768, 32767] to [0, 255]
    mono = (mono + 32768) * 255 / 65535;

    // Step 3: Clamp the value to ensure it fits in uint8_t
    if (mono < 0) mono = 0;
    if (mono > 255) mono = 255;

    return (uint8_t)mono;
}

uint8_t combine_to_mono_uint8(int16_t left, int16_t right) {
    // Step 1: Average the left and right channels
    int32_t mono = (left + right) / 2;

    // Step 2: Normalize to uint8_t range (0 to 255)
    // Shift the range from [-32768, 32767] to [0, 255]
    mono = (mono + 32768) * 255 / 65535;

    // Step 3: Clamp the value to ensure it fits in uint8_t
    if (mono < 0) mono = 0;
    if (mono > 255) mono = 255;

    return (uint8_t)mono;
}

int8_t combine_to_mono_int8(int16_t left, int16_t right) {
    // Step 1: Average the left and right channels
    int32_t mono = (left + right) / 2;

    // Step 2: Normalize to int8_t range (-128 to 127)
    // Scale from [-32768, 32767] to [-128, 127]
    mono = mono * 127 / 32767;

    // Step 3: Clamp the value to ensure it fits in int8_t
    if (mono < -128) mono = -128;
    if (mono > 127) mono = 127;

    return (int8_t)mono;
}

// Call the MOD player to fill the output audio buffer.
// This must be called every 20 miliseconds or so, or more
// often if SOUND_OUTPUT_FREQUENCY is increased.
static void update_mod_player(uint8_t* audio_buffer)
{
    mod_player_status = RenderMOD(modBuffer,AUDIO_BUFFER_SIZE);
    // int16_t* tip = modBuffer;
    // int16_t* tip_mixerbuffer = audio_get_mixerbuffer();
    // for (int i=0;i<AUDIO_BUFFER_SIZE;i++){
    //     int16_t left_channel = *tip++;
    //     int16_t right_channel = *tip++;
    //     //int16_t combine = (left_channel + right_channel) >> 2;
    //     int16_t combine = left_channel;

    //     *tip_mixerbuffer = combine >> 2;
    //     tip_mixerbuffer++;
    // } 

    int16_t* tip = modBuffer;
    int16_t* tip_mixerbuffer = audio_get_mixerbuffer();
    for (int i=0;i<AUDIO_BUFFER_SIZE;i++){
        int16_t left_channel = *tip++;
        int16_t right_channel = *tip++;
        int16_t combine = (int16_t)combine_to_mono_int8(left_channel,right_channel);
        //int8_t combine = left_channel;

        *tip_mixerbuffer = combine;
        tip_mixerbuffer++;
    } 

}

void audio_play_wav(uint8_t asset_id, bool loop) {
  const void* music_ptr = assets_get_pointer(asset_id);
  const uint32_t music_size = assets_get_size(asset_id);
  int source_music = loop 
                        ? audio_play_loop(music_ptr,music_size,0)
                        : audio_play_once(music_ptr,music_size);
}

void audio_play_mod(uint8_t asset_id, bool loop)
{
    void* ptr = assets_get_pointer(asset_id);
    const uint32_t size = assets_get_size(asset_id);
    
    mod_player_status = InitMOD(ptr,SAMPLERATE);
    mod_loop = loop;
  //mod_play_start(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY, 1);
  // mod_play_start(mod_data, frequency, loop ? 1 : 0);
}

bool mod_playing = true;

void audio_update()
{
  uint8_t *audio_buffer = audio_get_buffer();
  if (audio_buffer){
    update_mod_player(audio_buffer);
    //update_tsf();
    audio_mixer_step(audio_buffer, mod_playing);
  }

}