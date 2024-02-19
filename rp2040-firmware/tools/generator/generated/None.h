

#ifndef __NG_API_H_
#define __NG_API_H_

#include "ng_config.h"

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
}

typedef struct call_prototype_t {
    uint8_t data;
} call_prototype_t;

typedef struct gfx_renderqueue_apply_t {
// returns: void

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_renderqueue_apply_t;

typedef struct gfx_set_palettecolor_t {
// returns: void
    uint8_t color_idx;
    uint16_t color565;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_set_palettecolor_t;

typedef struct gfx_get_palettecolor_t {
// returns: uint16_t
    uint8_t color_idx;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_get_palettecolor_t;

typedef struct gfx_set_palette_from_assset_t {
// returns: void
    uint8_t asset_id;
    uint8_t fill_unused_with_idx;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_set_palette_from_assset_t;

typedef struct gfx_set_font_from_asset_t {
// returns: void
    uint8_t asset_id;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_set_font_from_asset_t;

typedef struct gfx_get_pixel_t {
// returns: uint8_t
    uint16_t x;
    uint16_t y;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_get_pixel_t;

typedef struct gfx_draw_pixel_t {
// returns: void
    uint16_t x;
    uint16_t y;
    uint8_t color_idx;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_draw_pixel_t;

typedef struct gfx_draw_char_t {
// returns: void
    uint16_t x;
    uint16_t y;
    char ch;
    uint8_t color_idx;

    uint8_t asset_id;
    uint8_t fillempty_with;
} gfx_draw_char_t;

#endif
