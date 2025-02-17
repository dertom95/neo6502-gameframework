

#include "ng_api.h"

const volatile uint8_t* mem_call_function = (uint8_t*)MM_FUNC_CALL;
const volatile uint8_t* mem_call_result   = (uint8_t*)MEMORY_MAP_CALLRETURN_BUFFER_BEGIN;

// returns: void f-grp:1 f-id:1
void asset_get_tilesheet(gfx_tilesheet_data_t* ts_data,uint8_t asset_id){
    uint8_t result_code;
    call_asset_get_tilesheet_t* func_data;
    func_data = (call_asset_get_tilesheet_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 4;
    func_data->hdr.func_id = 1;
    func_data->ts_data=(uint16_t) ts_data;
    func_data->asset_id= asset_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:2
void asset_get_tilemap(gfx_tilemap_t* tilemap,uint8_t asset_id){
    uint8_t result_code;
    call_asset_get_tilemap_t* func_data;
    func_data = (call_asset_get_tilemap_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 4;
    func_data->hdr.func_id = 2;
    func_data->tilemap=(uint16_t) tilemap;
    func_data->asset_id= asset_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   

// returns: uint8_t f-grp:1 f-id:5
uint8_t audio_wav_load(uint8_t asset_id){
    uint8_t result_code;
    call_audio_wav_load_t* func_data;
    func_data = (call_audio_wav_load_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 5;
    func_data->asset_id= asset_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;

        return call_result;
    }
}   
// returns: void f-grp:1 f-id:1
void audio_wav_play(uint8_t sound_id, bool loop){
    uint8_t result_code;
    call_audio_wav_play_t* func_data;
    func_data = (call_audio_wav_play_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 1;
    func_data->sound_id= sound_id;
    func_data->loop= loop;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:4
void audio_wav_stop(uint8_t sound_id){
    uint8_t result_code;
    call_audio_wav_stop_t* func_data;
    func_data = (call_audio_wav_stop_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 4;
    func_data->sound_id= sound_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:2
void audio_mod_play(uint8_t asset_id){
    uint8_t result_code;
    call_audio_mod_play_t* func_data;
    func_data = (call_audio_mod_play_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 2;
    func_data->asset_id= asset_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:7
void audio_mod_pause(void){
    uint8_t result_code;
    call_audio_mod_pause_t* func_data;
    func_data = (call_audio_mod_pause_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 7;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:8
void audio_mod_resume(void){
    uint8_t result_code;
    call_audio_mod_resume_t* func_data;
    func_data = (call_audio_mod_resume_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 8;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:3
void audio_mod_stop(){
    uint8_t result_code;
    call_audio_mod_stop_t* func_data;
    func_data = (call_audio_mod_stop_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 3;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: uint8_t f-grp:1 f-id:6
uint8_t audio_mod_pos(){
    uint8_t result_code;
    call_audio_mod_pos_t* func_data;
    func_data = (call_audio_mod_pos_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 5;
    func_data->hdr.func_id = 6;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;

        return call_result;
    }
}   

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
// returns: void f-grp:1 f-id:44
void gfx_renderqueue_wipe(void){
    uint8_t result_code;
    call_gfx_renderqueue_wipe_t* func_data;
    func_data = (call_gfx_renderqueue_wipe_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 44;

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
// returns: uint8_t f-grp:1 f-id:16
uint8_t gfx_spritebuffer_create(gfx_sprite_t* spritedata,uint8_t spriteamount){
    uint8_t result_code;
    call_gfx_spritebuffer_create_t* func_data;
    func_data = (call_gfx_spritebuffer_create_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 16;
    func_data->spritedata=(uint16_t) spritedata;
    func_data->spriteamount= spriteamount;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;

        return call_result;
    }
}   
// returns: void f-grp:1 f-id:21
void    gfx_spritebuffer_update(int16_t dt,uint8_t spritebuffer_id){
    uint8_t result_code;
    call_gfx_spritebuffer_update_t* func_data;
    func_data = (call_gfx_spritebuffer_update_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 21;
    func_data->dt= dt;
    func_data->spritebuffer_id= spritebuffer_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:27
void    gfx_sprite_apply_data(gfx_sprite_t* sprite){
    uint8_t result_code;
    call_gfx_sprite_apply_data_t* func_data;
    func_data = (call_gfx_sprite_apply_data_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 27;
    func_data->sprite=(uint16_t) sprite;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:17
void    gfx_sprite_set_tileset(gfx_sprite_t* sprite, gfx_tilesheet_data_t* tsdata, uint8_t initial_tile_idx){
    uint8_t result_code;
    call_gfx_sprite_set_tileset_t* func_data;
    func_data = (call_gfx_sprite_set_tileset_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 17;
    func_data->sprite=(uint16_t) sprite;
    func_data->tsdata=(uint16_t) tsdata;
    func_data->initial_tile_idx= initial_tile_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:18
void    gfx_sprite_set_tileid(gfx_sprite_t* sprite,uint8_t tile_idx){
    uint8_t result_code;
    call_gfx_sprite_set_tileid_t* func_data;
    func_data = (call_gfx_sprite_set_tileid_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 18;
    func_data->sprite=(uint16_t) sprite;
    func_data->tile_idx= tile_idx;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: uint8_t f-grp:1 f-id:19
uint8_t gfx_sprite_add_animator(gfx_sprite_t* sprite, gfx_sprite_animator_t* animator){
    uint8_t result_code;
    call_gfx_sprite_add_animator_t* func_data;
    func_data = (call_gfx_sprite_add_animator_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 19;
    func_data->sprite=(uint16_t) sprite;
    func_data->animator=(uint16_t) animator;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint8_t call_result = (uint8_t)*mem_call_result;

        return call_result;
    }
}   
// returns: bool f-grp:1 f-id:20
bool    gfx_sprite_remove_extension(gfx_sprite_t* sprite,uint8_t extension_type){
    uint8_t result_code;
    call_gfx_sprite_remove_extension_t* func_data;
    func_data = (call_gfx_sprite_remove_extension_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 20;
    func_data->sprite=(uint16_t) sprite;
    func_data->extension_type= extension_type;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        bool call_result = (bool)*mem_call_result;

        return call_result;
    }
}   
// returns: void f-grp:1 f-id:22
void    gfx_spriteanimator_set_animation(uint8_t spriteanimator_id, uint8_t anim_idx, uint8_t flags){
    uint8_t result_code;
    call_gfx_spriteanimator_set_animation_t* func_data;
    func_data = (call_gfx_spriteanimator_set_animation_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 22;
    func_data->spriteanimator_id= spriteanimator_id;
    func_data->anim_idx= anim_idx;
    func_data->flags= flags;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:23
void    gfx_spriteanimator_set_animation_with_folowup(uint8_t spriteanimator_id, uint8_t anim_idx, uint8_t flags, uint8_t followup_animation_idx, uint8_t followup_animation_flags){
    uint8_t result_code;
    call_gfx_spriteanimator_set_animation_with_folowup_t* func_data;
    func_data = (call_gfx_spriteanimator_set_animation_with_folowup_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 23;
    func_data->spriteanimator_id= spriteanimator_id;
    func_data->anim_idx= anim_idx;
    func_data->flags= flags;
    func_data->followup_animation_idx= followup_animation_idx;
    func_data->followup_animation_flags= followup_animation_flags;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:24
void    gfx_spriteanimator_stop(uint8_t spriteanimator_id){
    uint8_t result_code;
    call_gfx_spriteanimator_stop_t* func_data;
    func_data = (call_gfx_spriteanimator_stop_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 24;
    func_data->spriteanimator_id= spriteanimator_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:25
void    gfx_spriteanimator_resume(uint8_t spriteanimator_id){
    uint8_t result_code;
    call_gfx_spriteanimator_resume_t* func_data;
    func_data = (call_gfx_spriteanimator_resume_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 25;
    func_data->spriteanimator_id= spriteanimator_id;

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

        gfx_pixelbuffer_t* call_result = (gfx_pixelbuffer_t*)*(uint16_t*)mem_call_result;
        return call_result;
    }
}   
// returns: void f-grp:1 f-id:13
void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination, uint16_t page_size){
    uint8_t result_code;
    call_gfx_pixelbuffer_mount_t* func_data;
    func_data = (call_gfx_pixelbuffer_mount_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 13;
    func_data->pxb=(uint16_t) pxb;
    func_data->destination= destination;
    func_data->page_size= page_size;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: bool f-grp:1 f-id:33
bool gfx_mount_set_page(uint8_t mount_id, uint8_t page){
    uint8_t result_code;
    call_gfx_mount_set_page_t* func_data;
    func_data = (call_gfx_mount_set_page_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 33;
    func_data->mount_id= mount_id;
    func_data->page= page;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        bool call_result = (bool)*mem_call_result;

        return call_result;
    }
}   
// returns: void f-grp:1 f-id:34
void gfx_pixelbuffer_wipe(gfx_pixelbuffer_t* pxb, uint8_t wipe_value){
    uint8_t result_code;
    call_gfx_pixelbuffer_wipe_t* func_data;
    func_data = (call_gfx_pixelbuffer_wipe_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 34;
    func_data->pxb=(uint16_t) pxb;
    func_data->wipe_value= wipe_value;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:26
void gfx_pixelbuffer_apply_data(gfx_pixelbuffer_t* pxb){
    uint8_t result_code;
    call_gfx_pixelbuffer_apply_data_t* func_data;
    func_data = (call_gfx_pixelbuffer_apply_data_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 26;
    func_data->pxb=(uint16_t) pxb;

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
// returns: void f-grp:1 f-id:37
void    gfx_clip_rect(uint16_t x,uint16_t y, uint16_t w,uint16_t h){
    uint8_t result_code;
    call_gfx_clip_rect_t* func_data;
    func_data = (call_gfx_clip_rect_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 37;
    func_data->x= x;
    func_data->y= y;
    func_data->w= w;
    func_data->h= h;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:38
void    gfx_clip_clear(){
    uint8_t result_code;
    call_gfx_clip_clear_t* func_data;
    func_data = (call_gfx_clip_clear_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 38;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:39
void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color,uint8_t flags){
    uint8_t result_code;
    call_gfx_draw_rect_t* func_data;
    func_data = (call_gfx_draw_rect_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 39;
    func_data->x= x;
    func_data->y= y;
    func_data->width= width;
    func_data->height= height;
    func_data->color= color;
    func_data->flags= flags;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:40
void gfx_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color,uint8_t flags){
    uint8_t result_code;
    call_gfx_draw_circle_t* func_data;
    func_data = (call_gfx_draw_circle_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 40;
    func_data->x= x;
    func_data->y= y;
    func_data->radius= radius;
    func_data->color= color;
    func_data->flags= flags;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:41
void gfx_draw_button(gfx_button_t* btn){
    uint8_t result_code;
    call_gfx_draw_button_t* func_data;
    func_data = (call_gfx_draw_button_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 41;
    func_data->btn=(uint16_t) btn;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:8
void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx, uint8_t bg_index){
    uint8_t result_code;
    call_gfx_draw_char_t* func_data;
    func_data = (call_gfx_draw_char_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 8;
    func_data->x= x;
    func_data->y= y;
    func_data->ch= ch;
    func_data->color_idx= color_idx;
    func_data->bg_index= bg_index;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:14
void     gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx, uint8_t bg_index){
    uint8_t result_code;
    call_gfx_draw_text_t* func_data;
    func_data = (call_gfx_draw_text_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 14;
    func_data->x= x;
    func_data->y= y;
    func_data->txt=(uint16_t) txt;
    func_data->color_idx= color_idx;
    func_data->bg_index= bg_index;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:28
void gfx_tilesheet_current_set_pixel_size(uint8_t px_width,uint8_t px_height){
    uint8_t result_code;
    call_gfx_tilesheet_current_set_pixel_size_t* func_data;
    func_data = (call_gfx_tilesheet_current_set_pixel_size_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 28;
    func_data->px_width= px_width;
    func_data->px_height= px_height;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:29
void gfx_tilesheet_current_set(gfx_tilesheet_data_t* tsdata){
    uint8_t result_code;
    call_gfx_tilesheet_current_set_t* func_data;
    func_data = (call_gfx_tilesheet_current_set_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 29;
    func_data->tsdata=(uint16_t) tsdata;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:30
void  gfx_draw_tile(int16_t x, int16_t y,uint8_t tile_number){
    uint8_t result_code;
    call_gfx_draw_tile_t* func_data;
    func_data = (call_gfx_draw_tile_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 30;
    func_data->x= x;
    func_data->y= y;
    func_data->tile_number= tile_number;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:31
void gfx_draw_tilemap_layer(int16_t x,int16_t y, gfx_tilemap_layer_t* tilemap){
    uint8_t result_code;
    call_gfx_draw_tilemap_layer_t* func_data;
    func_data = (call_gfx_draw_tilemap_layer_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 31;
    func_data->x= x;
    func_data->y= y;
    func_data->tilemap=(uint16_t) tilemap;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:32
void gfx_load_tilemap_layer(gfx_tilemap_t* tm,gfx_tilemap_layer_t* out_layer, uint8_t layer_nr){
    uint8_t result_code;
    call_gfx_load_tilemap_layer_t* func_data;
    func_data = (call_gfx_load_tilemap_layer_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 32;
    func_data->tm=(uint16_t) tm;
    func_data->out_layer=(uint16_t) out_layer;
    func_data->layer_nr= layer_nr;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:42
void gfx_debug_drawinfo_pixelbuffer(uint16_t x, uint16_t y, gfx_pixelbuffer_t* pxb,uint8_t coltext,uint8_t col_bg){
    uint8_t result_code;
    call_gfx_debug_drawinfo_pixelbuffer_t* func_data;
    func_data = (call_gfx_debug_drawinfo_pixelbuffer_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 42;
    func_data->x= x;
    func_data->y= y;
    func_data->pxb=(uint16_t) pxb;
    func_data->coltext= coltext;
    func_data->col_bg= col_bg;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:43
void gfx_debug_drawinfo_keyboard(uint16_t x, uint16_t y, keyboard_mapping_t* keyb,uint8_t coltext, uint8_t col_bg){
    uint8_t result_code;
    call_gfx_debug_drawinfo_keyboard_t* func_data;
    func_data = (call_gfx_debug_drawinfo_keyboard_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 1;
    func_data->hdr.func_id = 43;
    func_data->x= x;
    func_data->y= y;
    func_data->keyb=(uint16_t) keyb;
    func_data->coltext= coltext;
    func_data->col_bg= col_bg;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   

// returns: void f-grp:1 f-id:8
void io_before_tick(void){
    uint8_t result_code;
    call_io_before_tick_t* func_data;
    func_data = (call_io_before_tick_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 8;

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
// returns: bool f-grp:1 f-id:6
bool io_gamepad_is_active(uint8_t gamepad_id){
    uint8_t result_code;
    call_io_gamepad_is_active_t* func_data;
    func_data = (call_io_gamepad_is_active_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 6;
    func_data->gamepad_id= gamepad_id;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        bool call_result = (bool)*mem_call_result;

        return call_result;
    }
}   
// returns: void f-grp:1 f-id:7
void io_input_clear_states(void){
    uint8_t result_code;
    call_io_input_clear_states_t* func_data;
    func_data = (call_io_input_clear_states_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 7;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:9
void io_lock_input(bool lock_it){
    uint8_t result_code;
    call_io_lock_input_t* func_data;
    func_data = (call_io_lock_input_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 2;
    func_data->hdr.func_id = 9;
    func_data->lock_it= lock_it;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   

// returns: uint16_t f-grp:1 f-id:2
uint16_t utils_random_uint16(){
    uint8_t result_code;
    call_utils_random_uint16_t* func_data;
    func_data = (call_utils_random_uint16_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 6;
    func_data->hdr.func_id = 2;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        uint16_t call_result = (uint16_t)*mem_call_result;

        return call_result;
    }
}   
// returns: void f-grp:1 f-id:1
void     ng_debug_value(uint16_t v1,uint16_t v2){
    uint8_t result_code;
    call_ng_debug_value_t* func_data;
    func_data = (call_ng_debug_value_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 6;
    func_data->hdr.func_id = 1;
    func_data->v1= v1;
    func_data->v2= v2;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   
// returns: void f-grp:1 f-id:3
void ng_debug_pointer(void* ptr, uint8_t data){
    uint8_t result_code;
    call_ng_debug_pointer_t* func_data;
    func_data = (call_ng_debug_pointer_t*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = 6;
    func_data->hdr.func_id = 3;
    func_data->ptr=(uint16_t) ptr;
    func_data->data= data;

    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {

    }
}   

