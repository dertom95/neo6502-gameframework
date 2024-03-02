

#include "ng_api.h"

const volatile uint8_t* mem_call_function = (uint8_t*)MM_FUNC_CALL;
const volatile uint8_t* mem_call_result   = (uint8_t*)MEMORY_MAP_CALLRETURN_BUFFER_BEGIN;

// returns: void f-grp:1 f-id:12
void gfx_renderqueue_add_id(uint8_t id){
    uint8_t result_code;
    call_gfx_renderqueue_add_id_t* func_data;
    func_data = (call_gfx_renderqueue_add_id_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 12;
    func_data->id= id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:1
void gfx_renderqueue_apply(void){
    uint8_t result_code;
    call_gfx_renderqueue_apply_t* func_data;
    func_data = (call_gfx_renderqueue_apply_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 1;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:9
void gfx_pixelbuffer_create(gfx_pixelbuffer_t* initial_data){
    uint8_t result_code;
    call_gfx_pixelbuffer_create_t* func_data;
    func_data = (call_gfx_pixelbuffer_create_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 9;
    func_data->initial_data=(uint16_t) initial_data;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:10
void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer){
    uint8_t result_code;
    call_gfx_pixelbuffer_set_active_t* func_data;
    func_data = (call_gfx_pixelbuffer_set_active_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 10;
    func_data->pxbuffer=(uint16_t) pxbuffer;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: gfx_pixelbuffer_t* f-grp:1 f-id:11
gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void){
    uint8_t result_code;
    call_gfx_pixelbuffer_get_current_t* func_data;
    func_data = (call_gfx_pixelbuffer_get_current_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 11;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        gfx_pixelbuffer_t* call_result = (gfx_pixelbuffer_t*)*mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:13
void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination){
    uint8_t result_code;
    call_gfx_pixelbuffer_mount_t* func_data;
    func_data = (call_gfx_pixelbuffer_mount_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 13;
    func_data->pxb=(uint16_t) pxb;
    func_data->destination= destination;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: uint8_t f-grp:1 f-id:16
uint8_t gfx_spritebuffer_create(gfx_sprite_t* spritedata,uint8_t spriteamount, bool set_current){
    uint8_t result_code;
    call_gfx_spritebuffer_create_t* func_data;
    func_data = (call_gfx_spritebuffer_create_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 16;
    func_data->spritedata=(uint16_t) spritedata;
    func_data->spriteamount= spriteamount;
    func_data->set_current= set_current;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:20
void gfx_spritebuffer_set_current(uint8_t spritebuffer_id){
    uint8_t result_code;
    call_gfx_spritebuffer_set_current_t* func_data;
    func_data = (call_gfx_spritebuffer_set_current_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 20;
    func_data->spritebuffer_id= spritebuffer_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: uint8_t f-grp:1 f-id:17
uint8_t gfx_sprite_set_tileset(uint8_t sprite_idx, uint8_t tileset_id, uint8_t initial_tile_idx){
    uint8_t result_code;
    call_gfx_sprite_set_tileset_t* func_data;
    func_data = (call_gfx_sprite_set_tileset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 17;
    func_data->sprite_idx= sprite_idx;
    func_data->tileset_id= tileset_id;
    func_data->initial_tile_idx= initial_tile_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:19
void gfx_sprite_set_tileid(uint8_t sprite_idx,uint8_t tile_idx){
    uint8_t result_code;
    call_gfx_sprite_set_tileid_t* func_data;
    func_data = (call_gfx_sprite_set_tileid_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 19;
    func_data->sprite_idx= sprite_idx;
    func_data->tile_idx= tile_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:2
void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565){
    uint8_t result_code;
    call_gfx_set_palettecolor_t* func_data;
    func_data = (call_gfx_set_palettecolor_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 2;
    func_data->color_idx= color_idx;
    func_data->color565= color565;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: uint16_t f-grp:1 f-id:3
uint16_t gfx_get_palettecolor(uint8_t color_idx){
    uint8_t result_code;
    call_gfx_get_palettecolor_t* func_data;
    func_data = (call_gfx_get_palettecolor_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 3;
    func_data->color_idx= color_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint16_t call_result = (uint16_t)*mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:4
void     gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx){
    uint8_t result_code;
    call_gfx_set_palette_from_assset_t* func_data;
    func_data = (call_gfx_set_palette_from_assset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 4;
    func_data->asset_id= asset_id;
    func_data->fill_unused_with_idx= fill_unused_with_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:5
void     gfx_set_font_from_asset(uint8_t asset_id){
    uint8_t result_code;
    call_gfx_set_font_from_asset_t* func_data;
    func_data = (call_gfx_set_font_from_asset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 5;
    func_data->asset_id= asset_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: uint8_t f-grp:1 f-id:6
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y){
    uint8_t result_code;
    call_gfx_get_pixel_t* func_data;
    func_data = (call_gfx_get_pixel_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 6;
    func_data->x= x;
    func_data->y= y;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:7
void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx){
    uint8_t result_code;
    call_gfx_draw_pixel_t* func_data;
    func_data = (call_gfx_draw_pixel_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 7;
    func_data->x= x;
    func_data->y= y;
    func_data->color_idx= color_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:8
void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx){
    uint8_t result_code;
    call_gfx_draw_char_t* func_data;
    func_data = (call_gfx_draw_char_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 8;
    func_data->x= x;
    func_data->y= y;
    func_data->ch= ch;
    func_data->color_idx= color_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:14
void     gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx){
    uint8_t result_code;
    call_gfx_draw_text_t* func_data;
    func_data = (call_gfx_draw_text_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 14;
    func_data->x= x;
    func_data->y= y;
    func_data->txt=(uint16_t) txt;
    func_data->color_idx= color_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:15
void  asset_get_tilesheet(gfx_tilesheet_data_t* ts_data,uint8_t asset_id){
    uint8_t result_code;
    call_asset_get_tilesheet_t* func_data;
    func_data = (call_asset_get_tilesheet_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 15;
    func_data->ts_data=(uint16_t) ts_data;
    func_data->asset_id= asset_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   

// returns: bool f-grp:1 f-id:1
bool io_keyboard_is_pressed(uint8_t keycode){
    uint8_t result_code;
    call_io_keyboard_is_pressed_t* func_data;
    func_data = (call_io_keyboard_is_pressed_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 1;
    func_data->keycode= keycode;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        bool call_result = (bool)*mem_call_result;
        return call_result;
    }
}   
// returns: bool f-grp:1 f-id:2
bool io_keyboard_is_down(uint8_t keycode){
    uint8_t result_code;
    call_io_keyboard_is_down_t* func_data;
    func_data = (call_io_keyboard_is_down_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 2;
    func_data->keycode= keycode;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        bool call_result = (bool)*mem_call_result;
        return call_result;
    }
}   
// returns: bool f-grp:1 f-id:3
bool io_keyboard_is_released(uint8_t keycode){
    uint8_t result_code;
    call_io_keyboard_is_released_t* func_data;
    func_data = (call_io_keyboard_is_released_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 3;
    func_data->keycode= keycode;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        bool call_result = (bool)*mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:4
void io_keyboardmapping_register(keyboard_mapping_t* address, uint8_t amount){
    uint8_t result_code;
    call_io_keyboardmapping_register_t* func_data;
    func_data = (call_io_keyboardmapping_register_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 4;
    func_data->address=(uint16_t) address;
    func_data->amount= amount;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:5
void   io_keyboardmapping_unregister(void){
    uint8_t result_code;
    call_io_keyboardmapping_unregister_t* func_data;
    func_data = (call_io_keyboardmapping_unregister_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 5;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   

