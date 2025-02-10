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


typedef struct gfx_tilesheet_t {
    gfx_tilesheet_data_t data;
	uint8_t** cached_tile_ptrs; // array of pointers to the 'cached' tiles. in pico these should be in RAM in x86 we can just point to the raw data
	uint8_t* tilesheet_data_raw;
} gfx_tilesheet_t;

// data to the current tilesheet to be used for painting into the current pixelbuffer
typedef struct gfx_tilesheet_current_info_t {
    gfx_tilesheet_t* ts;
    uint8_t px_width;
    uint8_t px_height;
    uint16_t tile_width;
    uint16_t tile_height;
} gfx_tilesheet_current_info_t;


#define GFX_EXTENSION_SPRITEANIMATOR 1

typedef struct gfx_extension_header_t {
    linked_list_t* next;
    uint8_t extension_type; // 
} gfx_extension_header_t;



typedef struct gfx_internal_sprite_t {
	gfx_tilesheet_t* tilesheet;
	void* tile_ptr; // direct link to the current tiledata
    gfx_extension_header_t* extensions; // manipualtors on this sprite
    // runtime precalculation
    uint16_t sprite_height;
    uint16_t sprite_width;

    int16_t sprite_x;
    int16_t sprite_y;

    int16_t  readbuf_offset;
    int16_t  writebuf_offset;

    uint8_t input_pixels_to_read;
    int8_t read_direction;
    
    uint8_t subpixel_left;    
    uint8_t subpixel_right;
    
    uint8_t format;
    uint8_t offset_left;

    uint8_t offset_top;
    uint8_t offset_right;

    uint8_t offset_bottom;
    uint8_t offset_width;

    uint8_t offset_height;
    uint8_t __unusedfield; // padding
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
    uint8_t current_anim_idx;
    uint8_t flags;

    uint8_t followup_anim_idx; // animation to be played if current animation is not looped
    uint8_t followup_flags;    
    uint8_t id;
    uint8_t free2; //  padding

    gfx_sprite_animator_t* sprite_animator;
    gfx_sprite_animation_t* current_animation;
    gfx_sprite_t* sprite;
} gfx_internal_sprite_animator_t;

typedef struct gfx_internal_tilemap_t {
    ng_mem_block_t mem;
    gfx_tilemap_t* tilemap_data;
    void* rawdata;
    gfx_tilemap_layer_t** layers;
} gfx_internal_tilemap_t;

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
/*api:1:21*/void    gfx_spritebuffer_update(int16_t dt,uint8_t spritebuffer_id);

/*api:1:27*/void    gfx_sprite_apply_data(gfx_sprite_t* sprite);
/*api:1:17*/void    gfx_sprite_set_tileset(gfx_sprite_t* sprite, gfx_tilesheet_data_t* tsdata, uint8_t initial_tile_idx);
/*api:1:18*/void    gfx_sprite_set_tileid(gfx_sprite_t* sprite,uint8_t tile_idx);
/*api:1:19*/uint8_t gfx_sprite_add_animator(gfx_sprite_t* sprite, gfx_sprite_animator_t* animator);
/*api:1:20*/bool    gfx_sprite_remove_extension(gfx_sprite_t* sprite,uint8_t extension_type);
/*api:1:22*/void    gfx_spriteanimator_set_animation(uint8_t spriteanimator_id, uint8_t anim_idx, uint8_t flags);
/*api:1:23*/void    gfx_spriteanimator_set_animation_with_folowup(uint8_t spriteanimator_id, uint8_t anim_idx, uint8_t flags, uint8_t followup_animation_idx, uint8_t followup_animation_flags);
/*api:1:24*/void    gfx_spriteanimator_stop(uint8_t spriteanimator_id);
/*api:1:25*/void    gfx_spriteanimator_resume(uint8_t spriteanimator_id);

// gets cached tile. caches it if it is not cached already (platform specific call)
void*    gfx_tilesheet_get_chached_tile(gfx_tilesheet_t* ts, uint8_t tile_id);

// 🇵​​​​​🇮​​​​​🇽​​​​​🇪​​​​​🇱​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​
// TODO: how to handle this id-framentation?
/*api:1:9*/void gfx_pixelbuffer_create(gfx_pixelbuffer_t* initial_data);
/*api:1:10*/void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer);
/*api:1:11*/gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void);
/*api:1:13*/void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination, uint16_t page_size);
/*api:1:33*/bool gfx_mount_set_page(uint8_t mount_id, uint8_t page);
/*api:1:34*/void gfx_pixelbuffer_wipe(gfx_pixelbuffer_t* pxb, uint8_t wipe_value);
/*api:1:26*/void gfx_pixelbuffer_apply_data(gfx_pixelbuffer_t* pxb);

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
/*api:1:8*/void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx, uint8_t bg_index);

// draw text. supports newline \n and color change: ##000 (color_idx). NEEDS TO BE THREE DIGITS! ##R (resets color_idx)
/*api:1:14*/void     gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx, uint8_t bg_index);
//void   gfx_draw_printf(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
//void     gfx_draw_printf(uint16_t x,uint16_t y,uint8_t color_idx, uint8_t bg_index,const char *format, ...);
void     gfx_tile_set_color(uint8_t x,uint8_t y,uint8_t color_idx);
/*api:1:28*/void gfx_tilesheet_current_set_pixel_size(uint8_t px_width,uint8_t px_height);
/*api:1:29*/void gfx_tilesheet_current_set(gfx_tilesheet_data_t* tsdata);
/*api:1:30*/void  gfx_draw_tile(int16_t x, int16_t y,uint8_t tile_number);
/*api:1:31*/void gfx_draw_tilemap_layer(int16_t x,int16_t y, gfx_tilemap_layer_t* tilemap);
/*api:1:32*/void gfx_load_tilemap_layer(gfx_tilemap_t* tm,gfx_tilemap_layer_t* out_layer, uint8_t layer_nr);
// write bitmap-asset to the current pixelbuffer to the specified pixel pos. px_size_4_4 to specify the pixelsize
// TODO: void gfx_draw_bitmap(int16_t x, int16_t y, uint8_t asset_id,uint8_t px_size_4_4);

// PLEASE: ALWAYS MAINTAIN: LAST API ID 1:34

#endif 