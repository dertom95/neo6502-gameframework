// NEOGAME-Firmware

#ifndef __GFX_H__
#define __GFX_H__

#include <stdint.h>
#include <stdarg.h>

#define MAP_WIDTH  512
#define MAP_HEIGHT 256



#define N_CHARACTERS 12


typedef struct {
	int16_t pos_x;
	int16_t pos_y;
	uint8_t tile;
	uint8_t tilestride;
	uint8_t ntiles;

	int16_t xmin, ymin;
	int16_t xmax, ymax;
	uint8_t dir;
	uint8_t anim_frame;
} character_t;

typedef struct {
	int16_t cam_x;
	int16_t cam_y;
	uint32_t frame_ctr;
	character_t chars[N_CHARACTERS];
} game_state_t;

typedef struct gfx_sprite_t {
	uint8_t width;
	uint8_t height;
	uint8_t tiles;
	uint8_t flags;
	void* data;
} gfx_sprite_t;

void 	 gfx_init();
void 	 gfx_draw();
void 	 gfx_update();
uint8_t* gfx_get_pixelbuffer(void);

void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565);
uint16_t gfx_get_palettecolor(uint8_t color_idx);
// set fontbuffer. the font needs to be 1bpp and 'character-aligned': first 8bytes for char1, then second,...
void     gfx_set_font(uint8_t* font_bpp1); 
// canvas functions
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y);
void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx);
void     gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx);
// draw text. supports newline \n and color change: ##000 (color_idx). NEEDS TO BE THREE DIGITS! ##R (resets color_idx)
void     gfx_draw_text(uint16_t x, uint16_t y, const char* txt, uint8_t color_idx);
//void   gfx_draw_printf(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
void     gfx_draw_printf(uint16_t x,uint16_t y,uint8_t color_idx,const char *format, ...);
void     gfx_tile_set_color(uint8_t x,uint8_t y,uint8_t color_idx);
#endif