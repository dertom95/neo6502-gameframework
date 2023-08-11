#include "ng_gfx.h"
#include "ng_sound.h"

#include "pico/stdlib.h"
#include "the_softliner.h"

#include<stdio.h>

int main(){
	stdio_init_all();
    
    gfx_init();
    sound_init(SOUND_OUTPUT_FREQUENCY_22K);
    sound_play_mod(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY_22K, true );

    while(1){
        gfx_draw();
        gfx_update();
        sound_update();
    }

	__builtin_unreachable();
}