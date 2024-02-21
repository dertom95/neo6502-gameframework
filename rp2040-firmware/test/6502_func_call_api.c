
#include<stdint.h>
#include "../source/api/gen/ng_api.h"
#include "../source/api/gen/ng_api.c"
#include "../mod/export/assets.h"


// void change_palette(uint8_t asset){
//     volatile uint8_t* trigger_function;
//     call_gfx_set_palette_from_assset_t* func_data;
//     uint8_t func_call_ok;

//     trigger_function = (uint8_t*)MM_FUNC_CALL;
//     func_data = (call_gfx_set_palette_from_assset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
//     func_data->function_id=1;
//     func_data->asset_id=asset;
//     func_data->fillempty_with=2;
    
//     func_call_ok = *trigger_function;
// }

// uint8_t get_pixel(uint16_t x, uint16_t y) {
//     volatile uint8_t* trigger_function;
//     gfx_get_pixel_t* func_data;
//     uint8_t func_call_ok;

//     trigger_function = (uint8_t*)MM_FUNC_CALL;
//     func_data = (gfx_get_pixel_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
//     func_data->func_id = 0x02;
//     func_data->x = x;
//     func_data->y = y;

//     func_call_ok = *trigger_function;

//     return func_call_ok;
// }

// void draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx) {
//     volatile uint8_t* trigger_function;
//     gfx_draw_pixel_t* func_data;
//     uint8_t func_call_ok;

//     trigger_function = (uint8_t*)MM_FUNC_CALL;
//     func_data = (gfx_draw_pixel_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
//     func_data->func_id = 0x03;
//     func_data->x = x;
//     func_data->y = y;
//     func_data->color_idx = color_idx;

//     func_call_ok = *trigger_function;
// }

// void draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx) {
//     volatile uint8_t* trigger_function;
//     gfx_draw_char_t* func_data;
//     uint8_t func_call_ok;

//     trigger_function = (uint8_t*)MM_FUNC_CALL;
//     func_data = (gfx_draw_char_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
//     func_data->func_id = 0x04;
//     func_data->x = x;
//     func_data->y = y;
//     func_data->ch = ch;
//     func_data->color_idx = color_idx;

//     func_call_ok = *trigger_function;
// }

int main(){
    uint8_t timer;
    uint8_t pos_x;
    uint8_t rows;
    uint8_t* tile_map = (uint8_t*)MEMORY_TILEAREA_BEGIN;
    uint8_t col;
    volatile uint16_t* mouse_x = (uint16_t*)MM_MOUSE_X;
    volatile uint16_t* mouse_y = (uint16_t*)MM_MOUSE_Y;

    uint8_t palette;

    palette = ASSET_COLOR_PALETTE;
    timer = 0;
    rows = 0;
    pos_x = 0;


    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,2);

    while (1){
        // //if(!(timer--))
        // {
        //     pos_x++;
        //     *(tile_map+10*40+pos_x)=pos_x+rows;        
        //     if (rows>=100){
        //         rows=0;
        //     }
        //     if (pos_x==0){
        //         rows++;
        //         palette = (palette == ASSET_COLOR_PALETTE_SMALL) ? ASSET_COLOR_PALETTE : ASSET_COLOR_PALETTE_SMALL;
        //         gfx_set_palette_from_assset(palette,1);                
        //     }
        //     //draw_char(20,20,'H',3);
        // }

        {
            uint8_t x = *mouse_x / 8;
            uint8_t y = *mouse_y / 8;
            *(tile_map+y*40+x)=col++;
        }

    }



    return 0;
}

// int main(){
//     uint8_t* tilegfx;
//     volatile uint8_t* key_pressed;

//     key_pressed = (uint8_t*)MEMORY_MAP_START;
//     tilegfx = (uint8_t*)MM_KEYSET;
    
//     {
//         int8_t posx,posy,col;
//         posx=0;
//         posy=0;
//         col=5;
//         while(1){
//             switch(*key_pressed){
//                 case 'a' : posx--; break;
//                 case 'd' : posx++; break;
//                 case 'w' : posy--; break;
//                 case 's' : posx++; break;
//             }

//             if (posx<0) posx=0;
//             if (posx>=MEMORY_TILES_WIDTH) posx=MEMORY_TILES_WIDTH-1;
//             if (posy<0) posy=0;
//             if (posy>=MEMORY_TILES_HEIGHT) posx=MEMORY_TILES_HEIGHT-1;

//             *(tilegfx+posx+posy*MEMORY_TILES_WIDTH)=col;
//         }
//     }
    
//     return 0;
// }