#include "ng_gfx.h"
#include "ng_sound.h"
#include "ng_utils.h"
#include "ng_io.h"

#include <pico/stdlib.h>
#include <signal.h>
#include "core/memory.h"
#include "core/roms.h"

#include<stdio.h>

#define SOUND

#ifdef SOUND
#include "../src/the_softliner.h"
#endif

// #define __BREAKPOINT__ raise(SIGINT);

int main(){
	loadROMS();
    //stdio_init_all();
    gfx_init();

    usb_init();
    
#ifdef SOUND
    sound_init(SOUND_OUTPUT_FREQUENCY_11K);
    sound_play_mod(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY_11K, true );
#endif    

    int last_millis = utils_millis();
    int last_sound_ms = 0;
    while(1){
        gfx_draw();
        gfx_update();
        
        gfx_draw_printf(0,0,COL_BLACK,"tuh_hid_report_received_cb %d:%d",address,data);

        usb_update();

        
        int current_millis = utils_millis();

#ifdef SOUND
        if (current_millis - last_sound_ms > 15){
            sound_update();
            last_sound_ms = current_millis;
        }
#endif

        int fps = 1000 / (current_millis - last_millis);
//        gfx_draw_printf(0,0,COL_BLACK,"fps:%d heap: total:%d free:%d",fps,utils_get_heap_total(),utils_get_heap_free());
        gfx_draw_printf(0,0,COL_BLACK,"fps:%d",fps);
        last_millis = current_millis;
    }

	__builtin_unreachable();
}