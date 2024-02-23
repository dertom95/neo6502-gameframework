
#include <stdint.h>
#include <ng_config.h>
#include <gen/ng_api.c>

#include "export/assets.h"

volatile uint16_t* mx =  (uint16_t*)MM_MOUSE_X;
volatile uint16_t* my =  (uint16_t*)MM_MOUSE_Y;
volatile uint8_t* mbtn = (uint8_t*)MM_MOUSE_BTN;

uint8_t* tile_map = (uint8_t*)MEMORY_TILEAREA_BEGIN;
uint8_t col = 0;
uint8_t current_back=0;
uint8_t current_x=0;
uint8_t current_y=0;

keyboard_mapping_t kbm[2];

int main(){
    kbm[0].key_state=8;
    kbm[0].flags=2;
    kbm[0].keycodes[0]=95;
    kbm[1].key_state=6;
    io_keyboardmapping_register(kbm,2);

    
    while(1){
        uint8_t x = *mx / 8;
        uint8_t y = *my / 8;

        if (current_x!=x || current_y!=y){
            *(tile_map+current_y*40+current_x)=current_back;        
            
            current_back = *(tile_map+y*40+x);
            //current_back = COL_GREEN;
            current_x=x;
            current_y=y;
        }
        {
            uint8_t btn_left = *mbtn;
            if ((btn_left&MOUSE_BUTTON_LEFT)>0){
                *(tile_map+y*40+x)=COL_RED;
                current_back=COL_RED;        
            } else {
                *(tile_map+y*40+x)=col++;        
            }
        }        

    }
    // asset_set_current_pack
    return 0;
}