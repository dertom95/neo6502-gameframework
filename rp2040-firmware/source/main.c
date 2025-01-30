#include "ng_gfx.h"
#include "ng_sound.h"
#include "ng_utils.h"
#include "ng_io.h"
#include "ng_cpu.h"
#include "ng_assets.h"
//#include "game/game.h"


//#include <pico/stdlib.h>
#include <stdint.h>
#include <signal.h>
#include "api/gen/ng_api.h"
#include "core/memory.h"
#include "core/roms.h"
#include "core/backend/cpu/wdc65C02cpu.h"


// LOADER! Move this somewhere else! Once we can load from usb
// #define INCLUDE_DATA
#ifdef INCLUDE_DATA
# include "../mod/export/assets.h"
# include "../mod/export/assets.c"
#endif
//--------------------------------------


#ifdef __KINC__
# include <kinc/system.h>
# include <stdio.h>
# include <kinc/threads/thread.h>
#elif PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
# include "pico/time.h"
#endif

int start_audio_test();

#include<stdio.h>

//#define SOUND

#ifdef SOUND
//#include "../src/the_softliner.h"
#endif

// #define __BREAKPOINT__ raise(SIGINT);

#ifdef __cplusplus
    extern "C" {
#endif


// TODO: clean this up
extern uint32_t tickscpu;
extern uint32_t frame;
extern uint32_t lasst_address;
extern uint8_t last_data;
#ifdef __KINC__
extern long long diff;
extern uint32_t ns_timer;
#endif




const uint32_t frame_len = 1000 / 30;

int32_t last_millis = 0;
int32_t last_sound_ms = 0;
int32_t msCount = 0;
int32_t tps = 0;
int32_t fps=0;
uint32_t tick_counter = 0;

#ifndef INCLUDE_DATA
void* assets_data = 0;
#endif
size_t assets_size = 0;

void main_init(){
    io_init();
    
    memory_init();
    gfx_init();  
   // game_init();
    // TODO: this have to be done by the loader! once it is established
#ifdef INCLUDE_DATA    
    assets_set_current_pack((void*)assets_data,sizeof(assets_data));
#else
    assets_set_current_pack(assets_data, assets_size);
#endif    

	loadROMS();


#ifdef SOUND

    sound_init(11000);
    //sound_play_mod(&mod_the_softliner, 11000, true );
#endif    
    ng_cpu_init();

    last_millis = utils_millis();
}


//stdio_init_all();
bool running = true;

void main_loop(void* data)
{
#if __KINC__
    kinc_thread_set_name("MAIN_LOOP");
#endif    

    while(running){
        int32_t current_millis = utils_millis();        
        int32_t msDelta = current_millis - last_millis;
        msCount += msDelta;
        *mm_ms_delta += msDelta; // memorymapped. the user can check here but have to reset the value to 0 manually
        tick_counter += msDelta;
        last_millis = current_millis;

        
//         if (msCount > 1000){
//             tps = (tickscpu);
//             tickscpu = 0;
//             msCount=-1000;
//             fps = frame;
//             frame = 0;
// //             gfx_draw_printf(0,0,COL_WHITE,"fps:%03d 6502:%07d addr:%04x data:%02x",fps,tps,last_address,last_data);
            
// // #ifdef __KINC__            
// //             gfx_draw_printf(0,10,COL_WHITE,"diff:%12d ns_timer:%07d ms:%05d",diff,ns_timer,msDelta);
// // #endif            
            
            
//             //gfx_draw_printf(0,40,COL_WHITE,"Ich bin Thomas!");            
//         }     
        //gfx_draw_printf(0,20,COL_WHITE,"current:%06d delta:%06d",current_millis,msDelta);
        //gfx_draw_printf(0,40,COL_WHITE,"msc:%06d tc:%06d",msCount,tick_counter);
        
       // gfx_draw_printf(0,20,COL_WHITE,"ticks:%06d counter:%d",tickscpu,mem[0x2000]);
        //printf("ticks:%06d counter:%d addr:%04x data:%02x\n",tickscpu,mem[0x2000],last_address,last_data);

        ng_cpu_update();
        if (tick_counter < frame_len){
            continue;
        }
        gfx_update();

        io_before_tick();
    // gfx_draw();
      //  game_tick(tick_counter);


        io_after_tick();
        


        // gfx_draw_pixel(mouse_x,mouse_y,current_col);

        


    #ifdef SOUND
        if (current_millis - last_sound_ms > 15){
            sound_update();
            last_sound_ms = current_millis;
        }
    #endif


        tick_counter = 0;
    }
}


#ifdef __KINC__
kinc_thread_t logic_thread;

void graphics_loop(void* data){
    gfx_backend_update();
}

int kickstart(int argc, char **argv){
    kinc_threads_init();

    main_init();

    // graphics thread is running on mainthread
    kinc_set_update_callback(graphics_loop,NULL);
    // logic on other thread
    kinc_thread_init(&logic_thread,main_loop,NULL);
	kinc_start();
}
#else
int main(){
    main_init();
    main_loop(NULL);
    return 0;
}
#endif




#ifdef __cplusplus
    }
#endif





// #include <stdio.h>
// #include <stdlib.h>
// #include "pico/stdlib.h"
// #include "pico/multicore.h"
// #include "hardware/clocks.h"
// #include "hardware/gpio.h"
// #include "hardware/irq.h"
// #include "hardware/sync.h"
// #include "hardware/vreg.h"
// #include "pico/sem.h"

// #include "dvi.h"
// #include "dvi_serialiser.h"
// #include "common_dvi_pin_configs.h"
// #include "sprite.h"

// // TMDS bit clock 252 MHz
// // DVDD 1.2V (1.1V seems ok too)
// #define FRAME_WIDTH 320
// #define FRAME_HEIGHT 240
// #define VREG_VSEL VREG_VOLTAGE_1_20
// #define DVI_TIMING dvi_timing_640x480p_60hz

// #define LED_PIN 16

// struct dvi_inst dvi0;
// uint16_t framebuf[FRAME_WIDTH * FRAME_HEIGHT];

// void core1_main() {
// 	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
// 	dvi_start(&dvi0);
// 	dvi_scanbuf_main_16bpp(&dvi0);
// 	__builtin_unreachable();
// }

// void core1_scanline_callback() {
// 	// Discard any scanline pointers passed back
// 	uint16_t *bufptr;
// 	while (queue_try_remove_u32(&dvi0.q_colour_free, &bufptr))
// 		;
// 	// // Note first two scanlines are pushed before DVI start
// 	static uint scanline = 2;
// 	bufptr = &framebuf[FRAME_WIDTH * scanline];
// 	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
// 	scanline = (scanline + 1) % FRAME_HEIGHT;
// }

// static const struct dvi_serialiser_cfg _pico_neo6502_cfg = {
// .pio = DVI_DEFAULT_PIO_INST,
// .sm_tmds = {0, 1, 2},
// .pins_tmds = {14, 18, 16},
// .pins_clk = 12,
// .invert_diffpairs = true
// };

// int main() {
// 	vreg_set_voltage(VREG_VSEL);
// 	sleep_ms(10);
// #ifdef RUN_FROM_CRYSTAL
// 	set_sys_clock_khz(12000, true);
// #else
// 	// Run system at TMDS bit clock
// 	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);
// #endif

// 	setup_default_uart();

// 	gpio_init(LED_PIN);
// 	gpio_set_dir(LED_PIN, GPIO_OUT);

// 	printf("Configuring DVI\n");

// 	dvi0.timing = &DVI_TIMING;
// 	dvi0.ser_cfg = _pico_neo6502_cfg;
// 	dvi0.scanline_callback = core1_scanline_callback;
// 	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

// 	// Once we've given core 1 the framebuffer, it will just keep on displaying
// 	// it without any intervention from core 0
// 	sprite_fill16(framebuf, 0xffff, FRAME_WIDTH * FRAME_HEIGHT);
// 	uint16_t *bufptr = framebuf;
// 	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
// 	bufptr += FRAME_WIDTH;
// 	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

// 	printf("Core 1 start\n");
// 	multicore_launch_core1(core1_main);

// 	printf("Start rendering\n");

// 	for (int y = 0; y < FRAME_HEIGHT; ++y) {
// 		for (int x = 0; x < FRAME_WIDTH; ++x) {
// 			const float scale = FRAME_HEIGHT / 2;
// 			float cr = ((float)x - FRAME_WIDTH / 2) / scale - 0.5f;
// 			float ci = ((float)y - FRAME_HEIGHT / 2) / scale;
// 			float zr = cr;
// 			float zi = ci;
// 			int iters;
// 			const int max_iters = 255;
// 			for (iters = 0; iters < max_iters; ++iters) {
// 				if (zr * zr + zi * zi > 4.f)
// 					break;
// 				float zrtemp = zr * zr - zi * zi + cr;
// 				zi = 2.f * zr * zi + ci;
// 				zr = zrtemp;
// 			}
// 			framebuf[y * FRAME_WIDTH + x] = ((max_iters - iters) >> 2) * 0x41 >> 1;
// 		}
// 	}
// 	while (1)
// 		__wfe();
// 	__builtin_unreachable();
// }

