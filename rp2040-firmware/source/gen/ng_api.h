

#ifndef __NG_API_H_
#define __NG_API_H_

#include "../api/ng_config.h"

#define MM_SB           MEMORY_MAP_START    // memory-location where the memory-mapping starts
#define MM_KEYSET       (MM_SB + 0x01)      // uint8
#define MM_MOUSE_X      (MM_SB + 0x02)      // uint16 
#define MM_MOUSE_Y      (MM_SB + 0x04)      // uint16
#define MM_MOUSE_BTN    (MM_SB + 0x06)      // uint8
#define MM_MOUSE_WHEEL  (MM_SB + 0x07)      // int8
#define MM_FUNC_CALL    (MM_SB + 0x08)      // uint8 

#define MOUSE_BUTTON_LEFT     = (1 << 0), ///< Left button
#define MOUSE_BUTTON_RIGHT    = (1 << 1), ///< Right button
#define MOUSE_BUTTON_MIDDLE   = (1 << 2), ///< Middle button
#define MOUSE_BUTTON_BACKWARD = (1 << 3), ///< Backward button,
#define MOUSE_BUTTON_FORWARD  = (1 << 4), ///< Forward button,

#define PALETTE_SIZE 255
#define COL_TRANSPARENT 0
#define COL_BLACK    1
#define COL_BLUE_DARK   2
#define COL_VIOLETTE 3
#define COL_GREEN_DARK    4
#define COL_ORANGE_DARK   5
#define COL_BROWNGREY 6
#define COL_LIGHTGREY 7
#define COL_WHITE     8
#define COL_RED       9  
#define COL_ORANGE   10
#define COL_YELLOW   11
#define COL_GREEN    12
#define COL_BLUE_LIGHT 13
#define COL_GREY 14
#define COL_PINK 15
#define COL_PEACH 16

typedef struct call_header_t {
    uint8_t func_type;
    uint8_t func_id;
} call_header_t;

 // returns: void f-grp:1 f-id:1;
void gfx_renderqueue_apply(void);
 // returns: void f-grp:1 f-id:2;
void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565);
 // returns: uint16_t f-grp:1 f-id:3;
uint16_t gfx_get_palettecolor(uint8_t color_idx);
 // returns: void f-grp:1 f-id:4;
void     gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx);
 // returns: void f-grp:1 f-id:5;
void     gfx_set_font_from_asset(uint8_t asset_id);
 // returns: uint8_t f-grp:1 f-id:6;
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y);
 // returns: void f-grp:1 f-id:7;
void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx);
 // returns: void f-grp:1 f-id:8;
void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx);
 // returns: bool f-grp:2 f-id:1;
bool io_keyboard_is_pressed(uint8_t keycode);
 // returns: bool f-grp:2 f-id:2;
bool io_keyboard_is_down(uint8_t keycode);
 // returns: bool f-grp:2 f-id:3;
bool io_keyboard_is_released(uint8_t keycode);

typedef struct call_prototype_t {
    uint8_t data;
} call_prototype_t;

// returns: void f-grp:1 f-id:1
typedef struct call_gfx_renderqueue_apply_t {
    call_header_t hdr;
} call_gfx_renderqueue_apply_t;
// returns: void f-grp:1 f-id:2
typedef struct call_gfx_set_palettecolor_t {
    call_header_t hdr;
    uint8_t color_idx;
    uint16_t color565;
} call_gfx_set_palettecolor_t;
// returns: uint16_t f-grp:1 f-id:3
typedef struct call_gfx_get_palettecolor_t {
    call_header_t hdr;
    uint8_t color_idx;
} call_gfx_get_palettecolor_t;
// returns: void f-grp:1 f-id:4
typedef struct call_gfx_set_palette_from_assset_t {
    call_header_t hdr;
    uint8_t asset_id;
    uint8_t fill_unused_with_idx;
} call_gfx_set_palette_from_assset_t;
// returns: void f-grp:1 f-id:5
typedef struct call_gfx_set_font_from_asset_t {
    call_header_t hdr;
    uint8_t asset_id;
} call_gfx_set_font_from_asset_t;
// returns: uint8_t f-grp:1 f-id:6
typedef struct call_gfx_get_pixel_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
} call_gfx_get_pixel_t;
// returns: void f-grp:1 f-id:7
typedef struct call_gfx_draw_pixel_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
    uint8_t color_idx;
} call_gfx_draw_pixel_t;
// returns: void f-grp:1 f-id:8
typedef struct call_gfx_draw_char_t {
    call_header_t hdr;
    uint16_t x;
    uint16_t y;
    char ch;
    uint8_t color_idx;
} call_gfx_draw_char_t;
// returns: bool f-grp:2 f-id:1
typedef struct call_io_keyboard_is_pressed_t {
    call_header_t hdr;
    uint8_t keycode;
} call_io_keyboard_is_pressed_t;
// returns: bool f-grp:2 f-id:2
typedef struct call_io_keyboard_is_down_t {
    call_header_t hdr;
    uint8_t keycode;
} call_io_keyboard_is_down_t;
// returns: bool f-grp:2 f-id:3
typedef struct call_io_keyboard_is_released_t {
    call_header_t hdr;
    uint8_t keycode;
} call_io_keyboard_is_released_t;

#endif
