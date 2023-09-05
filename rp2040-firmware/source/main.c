#include "ng_gfx.h"
#include "ng_sound.h"
#include "ng_utils.h"
#include "ng_io.h"

#include "pico/stdlib.h"
#include "the_softliner.h"

#include<stdio.h>

int main(){
	//stdio_init_all();
    
    gfx_init();

    usb_init();
    
    sound_init(SOUND_OUTPUT_FREQUENCY_22K);
    sound_play_mod(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY_22K, true );

    int last_millis = utils_millis();
    int last_sound_ms = 0;
    while(1){
        gfx_draw();
        gfx_update();
        
        usb_update();

        
        int current_millis = utils_millis();

        if (current_millis - last_sound_ms > 15){
            sound_update();
            last_sound_ms = current_millis;
        }


        int fps = 1000 / (current_millis - last_millis);
//        gfx_draw_printf(0,0,COL_BLACK,"fps:%d heap: total:%d free:%d",fps,utils_get_heap_total(),utils_get_heap_free());
        gfx_draw_printf(0,0,COL_BLACK,"fps:%d",fps);
        last_millis = current_millis;
    }

	__builtin_unreachable();
}