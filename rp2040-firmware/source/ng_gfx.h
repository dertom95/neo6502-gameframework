// NEOGAME-Firmware

#ifndef __GFX_H__
#define __GFX_H__

#include <stdint.h>
#include <stdarg.h>

#include "ng_mem.h"
#include "ng_defines.h"
#include "api/ng_api_shared.h"
#include "ng_utils.h"

#define TILESHEET_FORMAT_INDEXED = 1

typedef struct data_header_t {
	uint8_t type;
	uint8_t free;
	uint8_t free1;
	uint8_t free2;
	uint32_t size;
} data_header_t;

typedef struct gfx_tilesheet_t {
    gfx_tilesheet_data_t data;
	uint8_t** cached_tile_ptrs; // array of pointers to the 'cached' tiles. in pico these should be in RAM in x86 we can just point to the raw data
	uint8_t* tilesheet_data_raw;
} gfx_tilesheet_t;


#define GFX_EXTENSION_SPRITEANIMATOR 1

typedef struct gfx_extension_header_t {
    linked_list_t* next;
    uint8_t extension_type; // 
} gfx_extension_header_t;



typedef struct gfx_internal_sprite_t {
	gfx_tilesheet_t* tilesheet;
	void* tile_ptr; // direct link to the current tiledata
    gfx_extension_header_t* extensions; // manipualtors on this sprite
} gfx_internal_sprite_t;


typedef struct gfx_palette_t {
    uint16_t color_amount;
    uint16_t colors[];
} gfx_palette_t;

typedef struct __attribute__((aligned(4))) gfx_internal_spritebuffer_t {
    ng_mem_block_t mem;
    uint8_t obj_id;
    uint8_t amount_sprites;
    uint16_t flags;

    gfx_sprite_t* sprites;
    gfx_internal_sprite_t* sprite_internals;
} gfx_internal_spritebuffer_t;

typedef struct gfx_internal_sprite_animator_t {
    gfx_extension_header_t header;
    int16_t timer;
    uint8_t runtime_flags;
    uint8_t current_anim_idx;
    gfx_sprite_animator_t* sprite_animator;
    gfx_sprite_animation_t* current_animation;
} gfx_internal_sprite_animator_t;

extern const uint8_t* font;

void 	 gfx_init();
void     gfx_backend_init();
void 	 gfx_draw();
void 	 gfx_update();
void     gfx_backend_update();
uint8_t* gfx_get_pixelbuffer(void);


/// @brief add renderblock to this renderqueue builder (the renderqueue must be applied when finished)
/// @param renderblock 
void gfx_renderqueue_add(ng_mem_block_t* renderblock);
/*api:1:12*/void gfx_renderqueue_add_id(uint8_t id);

/// @brief start over with the renderqueue builder
/// @param  
void gfx_renderqueue_wipe(void);
/// @brief tell's the renderer to render this queue from now on
/// @param  
/*api:1:1*/void gfx_renderqueue_apply(void);
void     gfx_render_scanline(uint16_t *pixbuf, uint8_t y);

// 🇸​​​​​🇵​​​​​🇷​​​​​🇮​​​​​🇹​​​​​🇪​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​
// create a spritebuffer that can manage the specified sprites that are created in usermemory
/*api:1:16*/uint8_t gfx_spritebuffer_create(gfx_sprite_t* spritedata,uint8_t spriteamount);
/*api:1:21*/void     gfx_spritebuffer_update(int16_t dt,uint8_t spritebuffer_id);

/*api:1:17*/void gfx_sprite_set_tileset(gfx_sprite_t* sprite, gfx_tilesheet_data_t* tsdata, uint8_t initial_tile_idx);
/*api:1:18*/void gfx_sprite_set_tileid(gfx_sprite_t* sprite,uint8_t tile_idx);
/*api:1:19*/uint8_t gfx_sprite_add_animator(gfx_sprite_t* sprite, gfx_sprite_animator_t* animator);
/*api:1:20*/bool    gfx_sprite_remove_extension(gfx_sprite_t* sprite,uint8_t extension_type);
/*api:1:22*/ void gfx_spriteanimator_set_animation(uint8_t sprite_animator, uint8_t anim_idx);

// gets cached tile. caches it if it is not cached already (platform specific call)
void*    gfx_tilesheet_get_chached_tile(gfx_tilesheet_t* ts, uint8_t tile_id);

// 🇵​​​​​🇮​​​​​🇽​​​​​🇪​​​​​🇱​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​
// TODO: how to handle this id-framentation?
/*api:1:9*/void gfx_pixelbuffer_create(gfx_pixelbuffer_t* initial_data);
/*api:1:10*/void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer);
/*api:1:11*/gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void);
/*api:1:13*/void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination);

// 🇵​​​​​🇽​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​ - 🇨​​​​​🇦​​​​​🇳​​​​​🇻​​​​​🇦​​​​​🇸​​​​​ 🇴​​​​​🇵​​​​​🇪​​​​​🇷​​​​​🇦​​​​​🇹​​​​​🇮​​​​​🇴​​​​​🇳​​​​​
/*api:1:2*/void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565);
/*api:1:3*/uint16_t gfx_get_palettecolor(uint8_t color_idx);
/*api:1:4*/void     gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx);
// set fontbuffer. the font needs to be 1bpp and 'character-aligned': first 8bytes for char1, then second,...
void     gfx_set_font(const uint8_t* font_bpp1); 
/*api:1:5*/void     gfx_set_font_from_asset(uint8_t asset_id);
// canvas functions
/*api:1:6*/uint8_t  gfx_get_pixel(uint16_t x, uint16_t y);
/*api:1:7*/void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx);
/*api:1:8*/void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx);

// draw text. supports newline \n and color change: ##000 (color_idx). NEEDS TO BE THREE DIGITS! ##R (resets color_idx)
/*api:1:14*/void     gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx);
//void   gfx_draw_printf(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
void     gfx_draw_printf(uint16_t x,uint16_t y,uint8_t color_idx,const char *format, ...);
void     gfx_tile_set_color(uint8_t x,uint8_t y,uint8_t color_idx);


// PLEASE: ALWAYS MAINTAIN: LAST API ID 1:22

#endif 