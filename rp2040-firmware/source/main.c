#include "ng_gfx.h"
#include "pico/stdlib.h"

#include<stdio.h>

int main(){
	stdio_init_all();
    
    gfx_init();

    while(1){
        gfx_draw();
        gfx_update();
    }

	__builtin_unreachable();
}