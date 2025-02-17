#include "ng_gfx.h"
#include "ng_audio.h"
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
# include "export/assets.h"
# include "export/assets.c"
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
//#include "3rd/audio/mod_data.h"
//#include "3rd/audio/the_softliner.h"
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
volatile uint32_t tick_counter = 0;

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

    ng_audio_init();
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

        bool short_frame = tick_counter < frame_len;
        if (!short_frame){
           // io_before_tick();
#ifdef _MOD_NATIVE_
            ng_cpu_update();
#endif            
        }

#ifndef _MOD_NATIVE_
        ng_cpu_update();
#endif        
        if (short_frame){
            continue;
        }
        gfx_update();

    // gfx_draw();
      //  game_tick(tick_counter);


        io_after_tick();
        


        // gfx_draw_pixel(mouse_x,mouse_y,current_col);

        


    #ifdef SOUND
        if (current_millis - last_sound_ms > 15){
            audio_update();
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

