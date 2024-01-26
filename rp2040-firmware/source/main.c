#include "ng_gfx.h"
#include "ng_sound.h"
#include "ng_utils.h"
#include "ng_io.h"

#include <pico/stdlib.h>
#include <signal.h>
#include "api/ng_api.h"
#include "core/memory.h"
#include "core/roms.h"


#include<stdio.h>

//#define SOUND

#ifdef SOUND
#include "../src/the_softliner.h"
#endif

// #define __BREAKPOINT__ raise(SIGINT);

#ifdef __cplusplus
    extern "C" {
#endif

void fill_tile(uint8_t tX,uint8_t tY,uint8_t col)
{
    for (int y=tY*8,yEnd=(tY*8+8);y<yEnd;y++){
        for (int x=tX*8,xEnd=(tX*8+8);x<xEnd;x++){
            gfx_draw_pixel(x,y,col);
        }
    }
}


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

    int16_t posx=20;
    int16_t posy=20;
    uint8_t current_col=COL_BLACK;
    while(1){
        gfx_draw();
        gfx_update();
        
        bool paint = false;
        if (io_keyboard_is_pressed(HID_KEY_A)){
            if (posx>0){
                posx--;
            }
            paint = true;
        }
        else if (io_keyboard_is_pressed(HID_KEY_D)){
            if (posx<40){
                posx++;
            }
            paint = true;
        }
        else if (io_keyboard_is_pressed(HID_KEY_W)){
            if (posy>0){
                posy--;
            }
            paint = true;
        }
        else if (io_keyboard_is_released(HID_KEY_S)){
            if (posy<30){
                posy++;
            }
            paint = true;
        }
        usb_update();
             
        if (paint){
            fill_tile(posx,posy,current_col);
            current_col++;
        }

        gfx_draw_pixel(mouse_x,mouse_y,current_col);

        
        int current_millis = utils_millis();

#ifdef SOUND
        if (current_millis - last_sound_ms > 15){
            sound_update();
            last_sound_ms = current_millis;
        }
#endif

        int fps = 1000 / (current_millis - last_millis);
//        gfx_draw_printf(0,0,COL_BLACK,"fps:%d heap: total:%d free:%d",fps,utils_get_heap_total(),utils_get_heap_free());
        gfx_draw_printf(0,0,COL_WHITE,"fps:%03d",fps);
        last_millis = current_millis;
    }

	__builtin_unreachable();
}

#ifdef __cplusplus
    }
#endif