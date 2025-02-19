#ifndef __NG_API_SHARED_H_
#define __NG_API_SHARED_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include "ng_config.h"
#include "ng_memory_map.h"

// the key was just pressed
#define KEYBMAP_FLAG_SCAN_KEY_PRESSED (1 << 0)
// the key is still pressed
#define KEYBMAP_FLAG_SCAN_KEY_DOWN (1 << 1)
// the key was just released
#define KEYBMAP_FLAG_SCAN_KEY_RELEASED (1 << 2)
#define KEYBMAP_FLAG_SCAN_ALL KEYBMAP_FLAG_SCAN_KEY_PRESSED|KEYBMAP_FLAG_SCAN_KEY_DOWN|KEYBMAP_FLAG_SCAN_KEY_RELEASED

// TODO: split those in API-Header of their domain!

// █ █▄░█ █▀█ █░█ ▀█▀
// █ █░▀█ █▀▀ █▄█ ░█░
typedef struct keyboard_mapping_t
{
    uint8_t key_pressed;
    uint8_t key_down;
    uint8_t key_released;
    uint8_t keycodes[8];
    uint8_t flags;
} keyboard_mapping_t;

typedef struct mouse_info_t 
{
    uint8_t pressed;
    uint8_t down;
    uint8_t released;
    uint8_t btn;
    int8_t wheel;
} mouse_info_t;

#define MOUSE_BTN_LEFT    (1 << 0)
#define MOUSE_BTN_RIGHT   (1 << 1)
#define MOUSE_BTN_MIDDLE  (1 << 2)

// mouseinfo:
#define MOUSE_BTN_PRESSED_LEFT   (1 << 0)
#define MOUSE_BTN_PRESSED_RIGHT  (1 << 1)
#define MOUSE_BTN_PRESSED_MIDDLE (1 << 2)
#define MOUSE_BTN_RELEASED_LEFT   (1 << 0)
#define MOUSE_BTN_RELEASED_RIGHT  (1 << 1)
#define MOUSE_BTN_RELEASED_MIDDLE (1 << 2)



// █▀▄▀█ █▀▀ █▀▄▀█ █▀█ █▀█ █▄█
// █░▀░█ ██▄ █░▀░█ █▄█ █▀▄ ░█░
typedef struct ng_mem_block_t
{
    uint16_t flags; // free[15-7] | segment_id bits[6-4] | usage_type bits[3-0]
    uint16_t unused1;
    uint32_t size;
    uint8_t *data; // pointer to the actual data
} ng_mem_block_t;

// █▀▀ █▀▀ ▀▄▀
// █▄█ █▀░ █░█

// TODO: someday
// #define PXB_WRAPMODE_MASK (3 << 0)
// #define PXB_WRAPMODE_NONE      0
// #define PXB_WRAPMODE_WRAP      1
// #define PXB_WRAPMODE_MIRROR    2
// //TODO: What was the name if the last color is just repeated? :thinking:
// #define PXB_WRAPMODE_LASTCOLOR 3 
// #define PXB_WRAPMODE(PXB_FLAG,MODE) \
//     ((PXB_FLAG&~PXB_WRAPMODE_MASK)|MODE)

#define PXBFLAG_DIRTY (1 << 0)
#define PXBFLAG_VISIBLE (1 << 1)


typedef struct TU_ATTR_PACKED
{
  uint8_t  x;         ///< Delta x  movement of left analog-stick
  uint8_t  y;         ///< Delta y  movement of left analog-stick
  uint8_t  z;         ///< Delta z  movement of right analog-joystick
  uint8_t  rz;        ///< Delta Rz movement of right analog-joystick
  uint8_t  rx;        ///< Delta Rx movement of analog left trigger
  uint8_t  ry;        ///< Delta Ry movement of analog right trigger
  uint8_t hat;       ///< Buttons mask for currently pressed buttons in the DPad/hat
  uint16_t buttons;  ///< Buttons mask for currently pressed buttons
  uint16_t buttons2;  ///< Buttons mask for currently pressed buttons
} gamepad_t;


// gamepad info
#define GPI_GAMEPAD1_ACTIVE (1 << 0)
#define GPI_GAMEPAD2_ACTIVE (1 << 1)
#define GPI_GAMEPAD3_ACTIVE (1 << 2)
#define GPI_GAMEPAD4_ACTIVE (1 << 3)

// directions
#define GP_D_UP    (1 << 0)
#define GP_D_DOWN  (1 << 1)
#define GP_D_LEFT  (1 << 2)
#define GP_D_RIGHT (1 << 3)
#define GP_STATE_IN_USE  (1 << 7)

// buttons
#define GP_BTN_TOP          (1 << 0)
#define GP_BTN_BOTTOM       (1 << 1)
#define GP_BTN_LEFT         (1 << 2)
#define GP_BTN_RIGHT        (1 << 3)
#define GP_BTN_REAR_LEFT    (1 << 4)
#define GP_BTN_REAR_RIGHT   (1 << 5)
#define GP_BTN_START        (1 << 6)
#define GP_BTN_SELECT       (1 << 7)

typedef struct gamepad_state_t {
    uint8_t controls; // see GP_D_* defines
    uint8_t buttons; // see GP_BTN_* defines
} gamepad_state_t;

typedef struct __attribute__((aligned(4))) gfx_pixelbuffer_t
{
    uint8_t obj_id;
    uint8_t user_value;

    int16_t x;
    int16_t y;

    // the width of the pixelbuffer! Not changeable after creation
    uint16_t width; 
    // the height of the pixelbuffer! Not changeable after creation
    uint16_t height;

    uint16_t flags;
    // runtime-data. after changing values you need to apply
    uint16_t full_width;

    uint16_t output_pixels_to_write;
    uint8_t output_subpixels_start;
    uint8_t output_subpixels_end;
    
    uint16_t input_pixels_to_read;
    int16_t writebuf_offset;

    uint16_t readbuf_offset;
    
    uint8_t pixel_size;
    uint8_t mount_id;
} gfx_pixelbuffer_t;

#define GFX_BTNSTATE_PRESSED (1 << 0)
typedef struct gfx_button_t {
    const char btn_caption[40];

    uint16_t x;
    uint16_t y;
    
    uint8_t flags;
    uint8_t color_index_txt;
    uint8_t color_index_bg;
    uint8_t unused3;
} gfx_button_t;

#define GFX_FLAG_FILLED     (1 << 0)
#define GFX_FLAG_CLIPCHECK  (1 << 1)

#define SPRITEFLAG_READY  (1 << 0)
#define SPRITEFLAG_FLIP_H (1 << 1)
#define SPRITEFLAG_FLIP_V (1 << 2)
// sprite data changed (e.g. position, pixel-size) and needs to be recalculated! This is mandatory for changes to showup
#define SPRITEFLAG_DIRTY  (1 << 7)
// the sprite is visible at the moment 
#define SPRITEFLAG_VISIBLE (1 << 8) 
//TODO: hmm, maybe alignment is the wrong term? (right<->left mixed up). :thinking: doesn't matter for now
#define SPRITEFLAG_ALIGNH_MASK   (3 << 3)
#define SPRITEFLAG_ALIGNH_LEFT   (0 << 3)
#define SPRITEFLAG_ALIGNH_CENTER (1 << 3)
#define SPRITEFLAG_ALIGNH_RIGHT  (2 << 3)

#define SPRITEFLAG_ALIGNV_MASK   (3 << 5)
#define SPRITEFLAG_ALIGNV_TOP    (0 << 5)
#define SPRITEFLAG_ALIGNV_CENTER (1 << 5)
#define SPRITEFLAG_ALIGNV_BOTTOM (2 << 5)


typedef struct  __attribute__((aligned(4))) gfx_sprite_t {
	int16_t x;
	int16_t y;
    // b[7]   DIRTY (calculate internal data)
    // b[5-6] VERTICAL ALIGNMENT
    // b[3-4] HORIZONTAL ALIGNMENT
    // b[2]   FLIP_VERTICAL
    // b[1]   FLIP_HORIZONTAL
    // b[0]   INUSE
	uint16_t flags; 
    
    uint8_t tile_idx; //TODO: implement some kind of automatic mapping to trigger set_tile-mechanism when writing to this address (in 6502world)
    uint8_t spritebuffer_id; // the spritebuffer this sprite belongs to
    
    uint8_t sprite_idx; 
    uint8_t pixel_size;
} gfx_sprite_t;


// animator is running but there is no animation at the moment
#define ANIMATIONFLAG_STOPPED (1 << 0) 
// loop the current animation
#define ANIMATIONFLAG_LOOP (1 << 1)
// play animation backwards
#define ANIMATIONFLAG_BACKWARDS (1 << 2)

typedef struct __attribute__((aligned(4))) sprite_animation_t {
    uint16_t delay_ms;
    uint8_t start_tile;
    uint8_t end_tile;
} gfx_sprite_animation_t;

typedef struct __attribute__((aligned(4))) gfx_sprite_animator_t {
    uint8_t animation_amount;
    uint8_t flags;
    
    uint16_t free;
    gfx_sprite_animation_t animations[];
} gfx_sprite_animator_t;

// CAUTIOUS: DO NOT CHANGE THE ORDER AND DO NOT ADD FIELDS.
//           The data for the tilesheets is created by the exporter. 
//           If you change this struct you also have to modify the exporter (ng_tool_tilesheet.py: encode_tiles(..) )
//           IMPORTANT: if add/removing to this struct you also need to maintain GFX_TILESHEET_DATA_SIZE-define (see below)
#define GFX_TILESHEETDATA_T_HEADER_SIZE 10
typedef struct __align4 gfx_tilesheet_data_t{
	uint8_t type;
	uint8_t tile_width;
	uint8_t tile_height;
	uint8_t tile_width_half;
	uint8_t tile_height_half;    
	uint8_t cols;
    uint8_t rows;
	uint8_t tile_amount;

	uint8_t flags;
    uint8_t ts_id;
} gfx_tilesheet_data_t;

// typedef struct __align4 gfx_tilemap_data_t {
//     // amount of tiles horizontally
//     uint8_t width;
//     // amount of tiles vertically
//     uint8_t height;
//     // the tilesheet
//     uint8_t tilesheet_id;
//     // the data
//     uint8_t data[];
// } gfx_tilemap_data_t;

#define GFX_TILEMAP_LAYER_T_HEADER_SIZE 2 /*read size in dataformat*/

typedef struct gfx_tilemap_layer_t {
    uint8_t tm_handle;
    uint8_t __unused_field;

    uint8_t layer_id;
    uint8_t flags;

    uint8_t data[];
} gfx_tilemap_layer_t;

#define GFX_TILEMAP_T_HEADER_SIZE 4
typedef struct gfx_tilemap_t{
    // data directly written from asset-data
    uint8_t layer_amount;
    uint8_t tileamount_horizontal;
    uint8_t tileamount_vertical;
    uint8_t flags;

    // runtime-data
    uint8_t tilesheet_id;
    uint8_t palette_id;
    uint8_t handle;
    uint8_t __unused_field;
} gfx_tilemap_t;


#if defined(_MOD_NATIVE_)
    #define MEMPTR(ADDRESS) (memory_resolve_address((uint16_t)(ADDRESS)))
#else
    #define MEMPTR(ADDRESS) ((uint8_t*)ADDRESS)
#endif

int ng_snprintf(char* str, uint8_t size, const char* format, ...);

#define flags_set(FLAGS,MASK) FLAGS |= (MASK);
#define flags_unset(FLAGS,MASK) FLAGS &= ~(MASK);
#define flags_isset(FLAGS,MASK) ((FLAGS & (MASK))==(MASK))
#define flags_isset_some(FLAGS,MASK) ((FLAGS & (MASK))>0)
#define flags_pack_4_4(HIGH,LOW) ((HIGH)<<4)|(LOW)
#define flags_unpack_4_4(INPUT,OUT_VAR_HIGH,OUT_VAR_LOW) OUT_VAR_HIGH=(INPUT>>4);OUT_VAR_LOW=(INPUT&15);
#define flags_mask_value(INPUT,MASK) (INPUT&(MASK))
#define flags_mask_value_is(INPUT,MASK,VALUE) ((INPUT&(MASK))==VALUE)


#define PALETTE_SIZE 255
#define COL_TRANSPARENT 255
#define COL_BLACK 1
#define COL_BLUE_DARK 2
#define COL_VIOLETTE 3
#define COL_GREEN_DARK 4
#define COL_ORANGE_DARK 5
#define COL_BROWNGREY 6
#define COL_LIGHTGREY 7
#define COL_WHITE 8
#define COL_RED 9
#define COL_ORANGE 10
#define COL_YELLOW 11
#define COL_GREEN 12
#define COL_BLUE_LIGHT 13
#define COL_GREY 14
#define COL_PINK 15
#define COL_PEACH 16

/*dark*/
#define COL_GREY_0_DARK 80 
#define COL_GREY_1 81
#define COL_GREY_2 82
#define COL_GREY_3 83
#define COL_GREY_4 84
#define COL_GREY_5 85
#define COL_GREY_6 86
/*very light*/
#define COL_GREY_7_LIGHT 87 

#define HID_KEY_NONE 0x00
#define HID_KEY_A 0x04
#define HID_KEY_B 0x05
#define HID_KEY_C 0x06
#define HID_KEY_D 0x07
#define HID_KEY_E 0x08
#define HID_KEY_F 0x09
#define HID_KEY_G 0x0A
#define HID_KEY_H 0x0B
#define HID_KEY_I 0x0C
#define HID_KEY_J 0x0D
#define HID_KEY_K 0x0E
#define HID_KEY_L 0x0F
#define HID_KEY_M 0x10
#define HID_KEY_N 0x11
#define HID_KEY_O 0x12
#define HID_KEY_P 0x13
#define HID_KEY_Q 0x14
#define HID_KEY_R 0x15
#define HID_KEY_S 0x16
#define HID_KEY_T 0x17
#define HID_KEY_U 0x18
#define HID_KEY_V 0x19
#define HID_KEY_W 0x1A
#define HID_KEY_X 0x1B
#define HID_KEY_Y 0x1C
#define HID_KEY_Z 0x1D
#define HID_KEY_1 0x1E
#define HID_KEY_2 0x1F
#define HID_KEY_3 0x20
#define HID_KEY_4 0x21
#define HID_KEY_5 0x22
#define HID_KEY_6 0x23
#define HID_KEY_7 0x24
#define HID_KEY_8 0x25
#define HID_KEY_9 0x26
#define HID_KEY_0 0x27
#define HID_KEY_ENTER 0x28
#define HID_KEY_ESCAPE 0x29
#define HID_KEY_BACKSPACE 0x2A
#define HID_KEY_TAB 0x2B
#define HID_KEY_SPACE 0x2C
#define HID_KEY_MINUS 0x2D
#define HID_KEY_EQUAL 0x2E
#define HID_KEY_BRACKET_LEFT 0x2F
#define HID_KEY_BRACKET_RIGHT 0x30
#define HID_KEY_BACKSLASH 0x31
#define HID_KEY_EUROPE_1 0x32
#define HID_KEY_SEMICOLON 0x33
#define HID_KEY_APOSTROPHE 0x34
#define HID_KEY_GRAVE 0x35
#define HID_KEY_COMMA 0x36
#define HID_KEY_PERIOD 0x37
#define HID_KEY_SLASH 0x38
#define HID_KEY_CAPS_LOCK 0x39
#define HID_KEY_F1 0x3A
#define HID_KEY_F2 0x3B
#define HID_KEY_F3 0x3C
#define HID_KEY_F4 0x3D
#define HID_KEY_F5 0x3E
#define HID_KEY_F6 0x3F
#define HID_KEY_F7 0x40
#define HID_KEY_F8 0x41
#define HID_KEY_F9 0x42
#define HID_KEY_F10 0x43
#define HID_KEY_F11 0x44
#define HID_KEY_F12 0x45
#define HID_KEY_PRINT_SCREEN 0x46
#define HID_KEY_SCROLL_LOCK 0x47
#define HID_KEY_PAUSE 0x48
#define HID_KEY_INSERT 0x49
#define HID_KEY_HOME 0x4A
#define HID_KEY_PAGE_UP 0x4B
#define HID_KEY_DELETE 0x4C
#define HID_KEY_END 0x4D
#define HID_KEY_PAGE_DOWN 0x4E
#define HID_KEY_ARROW_RIGHT 0x4F
#define HID_KEY_ARROW_LEFT 0x50
#define HID_KEY_ARROW_DOWN 0x51
#define HID_KEY_ARROW_UP 0x52
#define HID_KEY_NUM_LOCK 0x53
#define HID_KEY_KEYPAD_DIVIDE 0x54
#define HID_KEY_KEYPAD_MULTIPLY 0x55
#define HID_KEY_KEYPAD_SUBTRACT 0x56
#define HID_KEY_KEYPAD_ADD 0x57
#define HID_KEY_KEYPAD_ENTER 0x58
#define HID_KEY_KEYPAD_1 0x59
#define HID_KEY_KEYPAD_2 0x5A
#define HID_KEY_KEYPAD_3 0x5B
#define HID_KEY_KEYPAD_4 0x5C
#define HID_KEY_KEYPAD_5 0x5D
#define HID_KEY_KEYPAD_6 0x5E
#define HID_KEY_KEYPAD_7 0x5F
#define HID_KEY_KEYPAD_8 0x60
#define HID_KEY_KEYPAD_9 0x61
#define HID_KEY_KEYPAD_0 0x62
#define HID_KEY_KEYPAD_DECIMAL 0x63
#define HID_KEY_EUROPE_2 0x64
#define HID_KEY_APPLICATION 0x65
#define HID_KEY_POWER 0x66
#define HID_KEY_KEYPAD_EQUAL 0x67
#define HID_KEY_F13 0x68
#define HID_KEY_F14 0x69
#define HID_KEY_F15 0x6A
#define HID_KEY_F16 0x6B
#define HID_KEY_F17 0x6C
#define HID_KEY_F18 0x6D
#define HID_KEY_F19 0x6E
#define HID_KEY_F20 0x6F
#define HID_KEY_F21 0x70
#define HID_KEY_F22 0x71
#define HID_KEY_F23 0x72
#define HID_KEY_F24 0x73
#define HID_KEY_EXECUTE 0x74
#define HID_KEY_HELP 0x75
#define HID_KEY_MENU 0x76
#define HID_KEY_SELECT 0x77
#define HID_KEY_STOP 0x78
#define HID_KEY_AGAIN 0x79
#define HID_KEY_UNDO 0x7A
#define HID_KEY_CUT 0x7B
#define HID_KEY_COPY 0x7C
#define HID_KEY_PASTE 0x7D
#define HID_KEY_FIND 0x7E
#define HID_KEY_MUTE 0x7F
#define HID_KEY_VOLUME_UP 0x80
#define HID_KEY_VOLUME_DOWN 0x81
#define HID_KEY_LOCKING_CAPS_LOCK 0x82
#define HID_KEY_LOCKING_NUM_LOCK 0x83
#define HID_KEY_LOCKING_SCROLL_LOCK 0x84
#define HID_KEY_KEYPAD_COMMA 0x85
#define HID_KEY_KEYPAD_EQUAL_SIGN 0x86
#define HID_KEY_KANJI1 0x87
#define HID_KEY_KANJI2 0x88
#define HID_KEY_KANJI3 0x89
#define HID_KEY_KANJI4 0x8A
#define HID_KEY_KANJI5 0x8B
#define HID_KEY_KANJI6 0x8C
#define HID_KEY_KANJI7 0x8D
#define HID_KEY_KANJI8 0x8E
#define HID_KEY_KANJI9 0x8F
#define HID_KEY_LANG1 0x90
#define HID_KEY_LANG2 0x91
#define HID_KEY_LANG3 0x92
#define HID_KEY_LANG4 0x93
#define HID_KEY_LANG5 0x94
#define HID_KEY_LANG6 0x95
#define HID_KEY_LANG7 0x96
#define HID_KEY_LANG8 0x97
#define HID_KEY_LANG9 0x98
#define HID_KEY_ALTERNATE_ERASE 0x99
#define HID_KEY_SYSREQ_ATTENTION 0x9A
#define HID_KEY_CANCEL 0x9B
#define HID_KEY_CLEAR 0x9C
#define HID_KEY_PRIOR 0x9D
#define HID_KEY_RETURN 0x9E
#define HID_KEY_SEPARATOR 0x9F
#define HID_KEY_OUT 0xA0
#define HID_KEY_OPER 0xA1
#define HID_KEY_CLEAR_AGAIN 0xA2
#define HID_KEY_CRSEL_PROPS 0xA3
#define HID_KEY_EXSEL 0xA4
// RESERVED					                      0xA5-DF
#define HID_KEY_CONTROL_LEFT 0xE0
#define HID_KEY_SHIFT_LEFT 0xE1
#define HID_KEY_ALT_LEFT 0xE2
#define HID_KEY_GUI_LEFT 0xE3
#define HID_KEY_CONTROL_RIGHT 0xE4
#define HID_KEY_SHIFT_RIGHT 0xE5
#define HID_KEY_ALT_RIGHT 0xE6
#define HID_KEY_GUI_RIGHT 0xE7
#include <stdint.h>

#endif