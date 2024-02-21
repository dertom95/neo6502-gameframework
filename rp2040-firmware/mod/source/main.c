
#include <stdint.h>
#include <gen/ng_api.h>
#include <gen/ng_api.c>

#include "export/assets.h"

volatile uint16_t* mx =  MM_MOUSE_X;
volatile uint16_t* my =  MM_MOUSE_Y;
uint8_t* tile_map = (uint8_t*)MEMORY_TILEAREA_BEGIN;
uint8_t col = 0;

int main(){
    while(1){
        uint8_t x = *mx / 8;
        uint8_t y = *my / 8;
        *(tile_map+y*40+x)=col++;        
    }
    // asset_set_current_pack
    return 0;
}