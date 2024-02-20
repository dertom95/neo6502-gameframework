

#include "ng_api.h"

const volatile uint8_t* call_function = (uint8_t*)MM_FUNC_CALL;

// returns: void f-grp:1 f-id:1
void gfx_renderqueue_apply(void){
    uint8_t result_code;
    call_gfx_renderqueue_apply_t* func_data;
    func_data = (call_gfx_renderqueue_apply_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 1;

    result_code = *call_function;

}   
// returns: void f-grp:1 f-id:2
void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565){
    uint8_t result_code;
    call_gfx_set_palettecolor_t* func_data;
    func_data = (call_gfx_set_palettecolor_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 2;
    func_data->color_idx=color_idx;
    func_data->color565=color565;

    result_code = *call_function;

}   
// returns: uint16_t f-grp:1 f-id:3
uint16_t gfx_get_palettecolor(uint8_t color_idx){
    uint8_t result_code;
    call_gfx_get_palettecolor_t* func_data;
    func_data = (call_gfx_get_palettecolor_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 3;
    func_data->color_idx=color_idx;

    result_code = *call_function;
    return *((uint16_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN));
}   
// returns: void f-grp:1 f-id:4
void     gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx){
    uint8_t result_code;
    call_gfx_set_palette_from_assset_t* func_data;
    func_data = (call_gfx_set_palette_from_assset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 4;
    func_data->asset_id=asset_id;
    func_data->fill_unused_with_idx=fill_unused_with_idx;

    result_code = *call_function;

}   
// returns: void f-grp:1 f-id:5
void     gfx_set_font_from_asset(uint8_t asset_id){
    uint8_t result_code;
    call_gfx_set_font_from_asset_t* func_data;
    func_data = (call_gfx_set_font_from_asset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 5;
    func_data->asset_id=asset_id;

    result_code = *call_function;

}   
// returns: uint8_t f-grp:1 f-id:6
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y){
    uint8_t result_code;
    call_gfx_get_pixel_t* func_data;
    func_data = (call_gfx_get_pixel_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 6;
    func_data->x=x;
    func_data->y=y;

    result_code = *call_function;
    return *((uint8_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN));
}   
// returns: void f-grp:1 f-id:7
void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx){
    uint8_t result_code;
    call_gfx_draw_pixel_t* func_data;
    func_data = (call_gfx_draw_pixel_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 7;
    func_data->x=x;
    func_data->y=y;
    func_data->color_idx=color_idx;

    result_code = *call_function;

}   
// returns: void f-grp:1 f-id:8
void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx){
    uint8_t result_code;
    call_gfx_draw_char_t* func_data;
    func_data = (call_gfx_draw_char_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 8;
    func_data->x=x;
    func_data->y=y;
    func_data->ch=ch;
    func_data->color_idx=color_idx;

    result_code = *call_function;

}   
// returns: bool f-grp:2 f-id:1
bool io_keyboard_is_pressed(uint8_t keycode){
    uint8_t result_code;
    call_io_keyboard_is_pressed_t* func_data;
    func_data = (call_io_keyboard_is_pressed_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 1;
    func_data->keycode=keycode;

    result_code = *call_function;
    return *((bool*)(MEMORY_MAP_CALL_BUFFER_BEGIN));
}   
// returns: bool f-grp:2 f-id:2
bool io_keyboard_is_down(uint8_t keycode){
    uint8_t result_code;
    call_io_keyboard_is_down_t* func_data;
    func_data = (call_io_keyboard_is_down_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 2;
    func_data->keycode=keycode;

    result_code = *call_function;
    return *((bool*)(MEMORY_MAP_CALL_BUFFER_BEGIN));
}   
// returns: bool f-grp:2 f-id:3
bool io_keyboard_is_released(uint8_t keycode){
    uint8_t result_code;
    call_io_keyboard_is_released_t* func_data;
    func_data = (call_io_keyboard_is_released_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 3;
    func_data->keycode=keycode;

    result_code = *call_function;
    return *((bool*)(MEMORY_MAP_CALL_BUFFER_BEGIN));
}   

