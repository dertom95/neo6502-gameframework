

#ifndef __NG_API_H_
#define __NG_API_H_

#include "../ng_config.h"

typedef struct call_header_t {
    uint8_t func_type;
    uint8_t func_id;
} call_header_t;

#define MOUSE_BUTTON_LEFT      (1 << 0) /* Left button */
#define MOUSE_BUTTON_RIGHT     (1 << 1) /* Right button */
#define MOUSE_BUTTON_MIDDLE    (1 << 2) /* Middle button */
#define MOUSE_BUTTON_BACKWARD  (1 << 3) /* Backward button, */
#define MOUSE_BUTTON_FORWARD   (1 << 4) /* Forward button */

// function grp: 4

 // returns: void f-grp: f-id:1;
void  asset_get_tilesheet(gfx_tilesheet_data_t* ts_data,uint8_t asset_id);

// returns: void f-grp: f-id:1
typedef struct call_asset_get_tilesheet_t {
    call_header_t hdr;
    uint16_t ts_data;
    uint8_t asset_id;
} call_asset_get_tilesheet_t;

// function grp: 1

 // returns: void f-grp: f-id:12;
void gfx_renderqueue_add_id(uint8_t id);
 // returns: void f-grp: f-id:1;
void gfx_renderqueue_apply(void);
 // returns: uint8_t f-grp: f-id:16;
uint8_t gfx_spritebuffer_create(gfx_sprite_t* spritedata,uint8_t spriteamount);
 // returns: void f-grp: f-id:21;
void    gfx_spritebuffer_update(int16_t dt,uint8_t spritebuffer_id);
 // returns: void f-grp: f-id:27;
void    gfx_sprite_apply_data(gfx_sprite_t* sprite);
 // returns: void f-grp: f-id:17;
void    gfx_sprite_set_tileset(gfx_sprite_t* sprite, gfx_tilesheet_data_t* tsdata, uint8_t initial_tile_idx);
 // returns: void f-grp: f-id:18;
void    gfx_sprite_set_tileid(gfx_sprite_t* sprite,uint8_t tile_idx);
 // returns: uint8_t f-grp: f-id:19;
uint8_t gfx_sprite_add_animator(gfx_sprite_t* sprite, gfx_sprite_animator_t* animator);
 // returns: bool f-grp: f-id:20;
bool    gfx_sprite_remove_extension(gfx_sprite_t* sprite,uint8_t extension_type);
 // returns: void f-grp: f-id:22;
void    gfx_spriteanimator_set_animation(uint8_t spriteanimator_id, uint8_t anim_idx, uint8_t flags);
 // returns: void f-grp: f-id:23;
void    gfx_spriteanimator_set_animation_with_folowup(uint8_t spriteanimator_id, uint8_t anim_idx, uint8_t flags, uint8_t followup_animation_idx, uint8_t followup_animation_flags);
 // returns: void f-grp: f-id:24;
void    gfx_spriteanimator_stop(uint8_t spriteanimator_id);
 // returns: void f-grp: f-id:25;
void    gfx_spriteanimator_resume(uint8_t spriteanimator_id);
 // returns: void f-grp: f-id:9;
void gfx_pixelbuffer_create(gfx_pixelbuffer_t* initial_data);
 // returns: void f-grp: f-id:10;
void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer);
 // returns: gfx_pixelbuffer_t* f-grp: f-id:11;
gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void);
 // returns: void f-grp: f-id:13;
void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination);
 // returns: void f-grp: f-id:26;
void gfx_pixelbuffer_apply_data(gfx_pixelbuffer_t* pxb);
 // returns: void f-grp: f-id:2;
void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565);
 // returns: uint16_t f-grp: f-id:3;
uint16_t gfx_get_palettecolor(uint8_t color_idx);
 // returns: void f-grp: f-id:4;
void     gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx);
 // returns: void f-grp: f-id:5;
void     gfx_set_font_from_asset(uint8_t asset_id);
 // returns: uint8_t f-grp: f-id:6;
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y);
 // returns: void f-grp: f-id:7;
void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx);
 // returns: void f-grp: f-id:8;
void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx);
 // returns: void f-grp: f-id:14;
void     gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx);

// returns: void f-grp: f-id:12
typedef struct call_gfx_renderqueue_add_id_t {
    call_header_t hdr;
    uint8_t id;
} call_gfx_renderqueue_add_id_t;
// returns: void f-grp: f-id:1
typedef struct call_gfx_renderqueue_apply_t {
    call_header_t hdr;
} call_gfx_renderqueue_apply_t;
// returns: uint8_t f-grp: f-id:16
typedef struct call_gfx_spritebuffer_create_t {
    call_header_t hdr;
    uint16_t spritedata;
    uint8_t spriteamount;
} call_gfx_spritebuffer_create_t;
// returns: void f-grp: f-id:21
typedef struct call_gfx_spritebuffer_update_t {
    call_header_t hdr;
    int16_t dt;
    uint8_t spritebuffer_id;
} call_gfx_spritebuffer_update_t;
// returns: void f-grp: f-id:27
typedef struct call_gfx_sprite_apply_data_t {
    call_header_t hdr;
    uint16_t sprite;
} call_gfx_sprite_apply_data_t;
// returns: void f-grp: f-id:17
typedef struct call_gfx_sprite_set_tileset_t {
    call_header_t hdr;
    uint16_t sprite;
    uint16_t tsdata;
    uint8_t initial_tile_idx;
} call_gfx_sprite_set_tileset_t;
// returns: void f-grp: f-id:18
typedef struct call_gfx_sprite_set_tileid_t {
    call_header_t hdr;
    uint16_t sprite;
    uint8_t tile_idx;
} call_gfx_sprite_set_tileid_t;
// returns: uint8_t f-grp: f-id:19
typedef struct call_gfx_sprite_add_animator_t {
    call_header_t hdr;
    uint16_t sprite;
    uint16_t animator;
} call_gfx_sprite_add_animator_t;
// returns: bool f-grp: f-id:20
typedef struct call_gfx_sprite_remove_extension_t {
    call_header_t hdr;
    uint16_t sprite;
    uint8_t extension_type;
} call_gfx_sprite_remove_extension_t;
// returns: void f-grp: f-id:22
typedef struct call_gfx_spriteanimator_set_animation_t {
    call_header_t hdr;
    uint8_t spriteanimator_id;
    uint8_t anim_idx;
    uint8_t flags;
} call_gfx_spriteanimator_set_animation_t;
// returns: void f-grp: f-id:23
typedef struct call_gfx_spriteanimator_set_animation_with_folowup_t {
    call_header_t hdr;
    uint8_t spriteanimator_id;
    uint8_t anim_idx;
    uint8_t flags;
    uint8_t followup_animation_idx;
    uint8_t followup_animation_flags;
} call_gfx_spriteanimator_set_animation_with_folowup_t;
// returns: void f-grp: f-id:24
typedef struct call_gfx_spriteanimator_stop_t {
    call_header_t hdr;
    uint8_t spriteanimator_id;
} call_gfx_spriteanimator_stop_t;
// returns: void f-grp: f-id:25
typedef struct call_gfx_spriteanimator_resume_t {
    call_header_t hdr;
    uint8_t spriteanimator_id;
} call_gfx_spriteanimator_resume_t;
// returns: void f-grp: f-id:9
typedef struct call_gfx_pixelbuffer_create_t {
    call_header_t hdr;
    uint16_t initial_data;
} call_gfx_pixelbuffer_create_t;
// returns: void f-grp: f-id:10
typedef struct call_gfx_pixelbuffer_set_active_t {
    call_header_t hdr;
    uint16_t pxbuffer;
} call_gfx_pixelbuffer_set_active_t;
// returns: gfx_pixelbuffer_t* f-grp: f-id:11
typedef struct call_gfx_pixelbuffer_get_current_t {
    call_header_t hdr;
} call_gfx_pixelbuffer_get_current_t;
// returns: void f-grp: f-id:13
typedef struct call_gfx_pixelbuffer_mount_t {
    call_header_t hdr;
    uint16_t pxb;
    uint16_t destination;
} call_gfx_pixelbuffer_mount_t;
// returns: void f-grp: f-id:26
typedef struct call_gfx_pixelbuffer_apply_data_t {
    call_header_t hdr;
    uint16_t pxb;
} call_gfx_pixelbuffer_apply_data_t;
// returns: void f-grp: f-id:2
typedef struct call_gfx_set_palettecolor_t {
    call_header_t hdr;
    uint8_t color_idx;
    uint16_t color565;
} call_gfx_set_palettecolor_t;
// returns: uint16_t f-grp: f-id:3
typedef struct call_gfx_get_palettecolor_t {
    call_header_t hdr;
    uint8_t color_idx;
} call_gfx_get_palettecolor_t;
// returns: void f-grp: f-id:4
typedef struct call_gfx_set_palette_from_assset_t {
    call_header_t hdr;
    uint8_t asset_id;
    uint8_t fill_unused_with_idx;
} call_gfx_set_palette_from_assset_t;
// returns: void f-grp: f-id:5
typedef struct call_gfx_set_font_from_asset_t {
    call_header_t hdr;
    uint8_t asset_id;
} call_gfx_set_font_from_asset_t;
// returns: uint8_t f-grp: f-id:6
typedef struct call_gfx_get_pixel_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
} call_gfx_get_pixel_t;
// returns: void f-grp: f-id:7
typedef struct call_gfx_draw_pixel_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
    uint8_t color_idx;
} call_gfx_draw_pixel_t;
// returns: void f-grp: f-id:8
typedef struct call_gfx_draw_char_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
    char ch;
    uint8_t color_idx;
} call_gfx_draw_char_t;
// returns: void f-grp: f-id:14
typedef struct call_gfx_draw_text_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
    uint16_t txt;
    uint8_t color_idx;
} call_gfx_draw_text_t;

// function grp: 2

 // returns: bool f-grp: f-id:1;
bool io_keyboard_is_pressed(uint8_t keycode);
 // returns: bool f-grp: f-id:2;
bool io_keyboard_is_down(uint8_t keycode);
 // returns: bool f-grp: f-id:3;
bool io_keyboard_is_released(uint8_t keycode);
 // returns: void f-grp: f-id:4;
void io_keyboardmapping_register(keyboard_mapping_t* address, uint8_t amount);
 // returns: void f-grp: f-id:5;
void   io_keyboardmapping_unregister(void);

// returns: bool f-grp: f-id:1
typedef struct call_io_keyboard_is_pressed_t {
    call_header_t hdr;
    uint8_t keycode;
} call_io_keyboard_is_pressed_t;
// returns: bool f-grp: f-id:2
typedef struct call_io_keyboard_is_down_t {
    call_header_t hdr;
    uint8_t keycode;
} call_io_keyboard_is_down_t;
// returns: bool f-grp: f-id:3
typedef struct call_io_keyboard_is_released_t {
    call_header_t hdr;
    uint8_t keycode;
} call_io_keyboard_is_released_t;
// returns: void f-grp: f-id:4
typedef struct call_io_keyboardmapping_register_t {
    call_header_t hdr;
    uint16_t address;
    uint8_t amount;
} call_io_keyboardmapping_register_t;
// returns: void f-grp: f-id:5
typedef struct call_io_keyboardmapping_unregister_t {
    call_header_t hdr;
} call_io_keyboardmapping_unregister_t;

#endif
