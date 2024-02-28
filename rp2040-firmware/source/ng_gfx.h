// NEOGAME-Firmware

#ifndef __GFX_H__
#define __GFX_H__

#include <stdint.h>
#include <stdarg.h>

#include "ng_mem.h"
#include "ng_defines.h"
#include "api/ng_api_shared.h"

#define MAP_WIDTH  512
#define MAP_HEIGHT 256



#define N_CHARACTERS 0

#define MAX_SPRITES 64

typedef struct data_header_t {
	uint8_t type;
	uint8_t free;
	uint8_t free1;
	uint8_t free2;
	uint32_t size;
} data_header_t;

typedef struct gfx_tilesheet_t {
	uint8_t type;
	uint8_t tile_width;
	uint8_t tile_height;
	uint8_t cols;
	uint8_t rows;

	uint8_t tile_amount;

	uint16_t flags;
	uint8_t** cached_tile_ptrs; // array of pointers to the 'cached' tiles. in pico these should be in RAM in x86 we can just point to the raw data
	uint8_t* tilesheet_data_raw;
} gfx_tilesheet_t;

#define SPRITE_FLAG_INUSE (1 << 0)

typedef struct gfx_sprite_t {
	int16_t x;
	int16_t y;
	uint8_t tile_id;
	uint16_t flags; // flags = 0 => free to use
	gfx_tilesheet_t* tilesheet;
	void* tile_ptr; // direct link to the current tiledata
} gfx_sprite_t;


typedef struct gfx_palette_t {
    uint16_t color_amount;
    uint16_t colors[];
} gfx_palette_t;

typedef struct gfx_sprite_buffer_t {
	ng_mem_block_t mem;
	uint8_t max_sprites;
	uint8_t flags;
} gfx_sprite_buffer_t;

#define TILESHEET_FORMAT_INDEXED = 1


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

// TODO: how to handle this id-framentation?
/*api:1:9*/void gfx_pixelbuffer_create(gfx_pixelbuffer_t* initial_data);
/*api:1:10*/void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer);
/*api:1:11*/gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void);
/*api:1:13*/void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination);


bool gfx_spritebuffer_create(uint8_t segment_id, gfx_sprite_buffer_t* spritebuffer);
gfx_sprite_t* gfx_sprite_create_from_tilesheet(gfx_sprite_buffer_t* spritebuffer, int16_t x,int16_t y, gfx_tilesheet_t* ts);
void          gfx_sprite_set_tileid(gfx_sprite_t* sprite, uint8_t tile_id);
// gets cached tile. caches it if it is not cached already (platform specific call)
void*    gfx_tilesheet_get_chached_tile(gfx_tilesheet_t* ts, uint8_t tile_id);

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
void     gfx_draw_text(uint16_t x, uint16_t y, const char* txt, uint8_t color_idx);
//void   gfx_draw_printf(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
void     gfx_draw_printf(uint16_t x,uint16_t y,uint8_t color_idx,const char *format, ...);
void     gfx_tile_set_color(uint8_t x,uint8_t y,uint8_t color_idx);

void     gfx_render_scanline(uint16_t *pixbuf, uint8_t y);
gfx_tilesheet_t* asset_get_tilesheet(uint8_t asset_id);


// PLEASE: ALWAYS MAINTAIN: LAST API ID 1:13

#endif 