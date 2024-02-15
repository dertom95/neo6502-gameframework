#include "ng_gfx.h"

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
# include "pico/platform.h"
#endif

#include "gen/font_8.h"
#include "gen/color_palette.h"
//#include "zelda_mini_plus_walk_rgab5515.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ng_io.h"
#include "api/ng_api.h"
#include <math.h>

#include "ng_utils.h"
#include <assert.h>
#include "core/memory.h"
#include "ng_assets.h"

#include <stddef.h>

uint8_t default_allocation_segment = SEGMENT_GFX_DATA;


// TODO: this must be done somewhere else in userspace
gfx_pixelbuffer_t initial_pixelbuffer={
	.x=0,
	.y=0,
	.width=320,
	.height=100,
};


// bool __scratch_x("render") requested_renderqueue_apply = false;
// ng_mem_block_t* __scratch_x("render")renderqueue_core1[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
// ng_mem_block_t* __scratch_y("render")renderqueue_request[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
bool requested_renderqueue_apply = false;
ng_mem_block_t* renderqueue_1[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
ng_mem_block_t* renderqueue_2[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};

uint8_t renderqueue_request_amount=0;
ng_mem_block_t** renderqueue_current = renderqueue_1;
ng_mem_block_t** renderqueue_request = renderqueue_2;

uint8_t* font=NULL; 		  // 1bpp
gfx_pixelbuffer_t* active_pixelbuffer = NULL;

game_state_t state;

uint16_t gfx_color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}


static inline int clip(int x, int min, int max) {
	return x < min ? min : x > max ? max : x;
}

uint16_t* sp16 = NULL;



bool gfx_spritebuffer_create(uint8_t segment_id, gfx_sprite_buffer_t* spritebuffer)
{
	assert(spritebuffer->mem.data == NULL);

	uint8_t sprite_amount = spritebuffer -> max_sprites;

	uint16_t size = sizeof(gfx_sprite_t) * sprite_amount;

	bool success = ng_mem_allocate_block(segment_id, size, MEM_USAGE_SPRITEBUFFER, &spritebuffer->mem);
	assert(success);
	ng_memblock_wipe(&spritebuffer->mem);

	return success;
}

gfx_sprite_t* _gfx_sprite_find_free(gfx_sprite_buffer_t* spritebuffer)
{
	uint8_t max_count = spritebuffer->max_sprites;
	gfx_sprite_t* sprite = (gfx_sprite_t*)spritebuffer->mem.data;
	while (max_count--){
		if (sprite->flags==0){
			return sprite;
		}
		sprite++;
	}
	return NULL;
}


gfx_sprite_t* gfx_sprite_create_from_tilesheet(gfx_sprite_buffer_t* spritebuffer, int16_t x,int16_t y, gfx_tilesheet_t* ts)
{
	gfx_sprite_t* sprite = _gfx_sprite_find_free(spritebuffer);
	if (sprite == NULL){
		return NULL;
	}
	sprite->tilesheet = ts;
	sprite->x=x;
	sprite->y=y;
	sprite->flags = 1; // TODO
	sprite->tile_id = 255; // add value != 0 for the set_tileid to actually work
	gfx_sprite_set_tileid(sprite,0);

	return sprite;
}

void    gfx_sprite_set_tileid(gfx_sprite_t* sprite, uint8_t tile_id)
{
	if (tile_id == sprite->tile_id){
		return;
	}
	assert(tile_id < sprite->tilesheet->tile_amount && "exceeded tile-id");
	assert(sprite->tilesheet!=NULL && "no tilesheet set for sprite");
	
	sprite->tile_ptr = gfx_tilesheet_get_chached_tile(sprite->tilesheet, tile_id);
	sprite->tile_id = tile_id;
}

void gfx_tile_set_color(uint8_t tX,uint8_t tY,uint8_t col)
{
    int16_t _x = tX * 8;
	int16_t _y = tY * 8;
	if (_x < 0 || _y < 0 || _x>active_pixelbuffer->width || _y>active_pixelbuffer->height){
		return;
	}
	uint8_t width = min(8, active_pixelbuffer->width-_x-1);
	uint8_t height = min(8, active_pixelbuffer->height-_y-1);
	for (int y=_y,yEnd=(_y+height);y<yEnd;y++){
        for (int x=_x,xEnd=(_x+width);x<xEnd;x++){
            gfx_draw_pixel(x,y,col);
        }
    }
}

#include <stdint.h>


// SCANLINE-RENDERER
#ifdef PICO_NEO6502
void __not_in_flash_func(gfx_render_scanline)(uint16_t *pixbuf, uint8_t y)
#else
void gfx_render_scanline(uint16_t *pixbuf, uint8_t y)
#endif
{
	memset(pixbuf,0,SCREEN_WIDTH*sizeof(uint16_t)); // make this a renderqueue-command

	for (int idx = 0; idx < GFX_RENDERQUEUE_MAX_ELEMENTS; idx++){
		ng_mem_block_t* current_render_block = renderqueue_current[idx];
		if (current_render_block==NULL){
			break;
		}

		uint8_t usage = ng_memblock_get_usage(current_render_block);


		uint16_t* write_buf = pixbuf;

		switch(usage){
			case MEM_USAGE_PIXELBUFFER:{
				// █▀█ █ ▀▄▀ █▀▀ █░░ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█
				// █▀▀ █ █░█ ██▄ █▄▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄				

				gfx_pixelbuffer_t* pixelbuffer = (gfx_pixelbuffer_t*)current_render_block;

				int16_t pixel_y = y - pixelbuffer->y;

				bool is_visible = pixel_y >= 0 && pixel_y < pixelbuffer->height;

				// bool has_no_pixelbuffer_intersection = (pixelbuffer->y > 0 &&  (y < pixelbuffer->y || y > pixelbuffer->y+pixelbuffer->height))
				// 									   || (pixelbuffer->y < 0 && ( pixelbuffer->y+y < 0 || pixelbuffer->y+y > pixelbuffer->height));

				if (is_visible){
					//uint8_t* buffer = &pixelbuffer[y*320];
					uint16_t count;
					uint8_t* buffer = pixelbuffer->mem.data + pixel_y*pixelbuffer->width;
					
					if (pixelbuffer->x>0){
						write_buf+=pixelbuffer->x; 
						count = min(SCREEN_WIDTH-pixelbuffer->x, pixelbuffer->width);
					} else {
						// pixelbuffer->x is negative!
						buffer+=-pixelbuffer->x;
						count = pixelbuffer->width + pixelbuffer->x; 
					}
					while (count--){
						uint8_t data = *(buffer++);
						*(write_buf++)=color_palette[data];
					}
				}
				break;
			}

			case MEM_USAGE_SPRITEBUFFER: {
				// █▀ █▀█ █▀█ █ ▀█▀ █▀▀ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█
				// ▄█ █▀▀ █▀▄ █ ░█░ ██▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄				

				uint16_t* write_buf = pixbuf;
				gfx_sprite_buffer_t* spritebuffer = (gfx_sprite_buffer_t*)current_render_block;
				uint8_t max_sprites = spritebuffer->max_sprites;
				gfx_sprite_t* sprite = (gfx_sprite_t*)spritebuffer->mem.data;
				while(max_sprites--){
					if (sprite->flags>0){
						gfx_tilesheet_t* ts = sprite->tilesheet;
						if (sprite->y > y || (sprite->y+ts->tile_height)<y){
							sprite++;
							continue;
						}

						uint8_t count = min(sprite->tilesheet->tile_width,SCREEN_WIDTH-sprite->x);
						uint8_t* data = sprite->tile_ptr + (y - sprite->y)*ts->tile_width;
						write_buf = pixbuf+sprite->x;
						uint8_t idx;
						while (count--){
							idx = *(data++);
							if (idx==255){
								write_buf++;
								continue;
							}
							*(write_buf++)=color_palette[idx];
						}		
					}
					sprite++;
				}
				break;
			}
		}

	}


	// for (int i = 0; i < N_CHARACTERS; ++i) {
	// 	const character_t *ch = &gstate->chars[i];
	// 	gfx_sprite_t* sprite = &test_sprite;

	// 	if (ch->pos_y > y || (ch->pos_y+sprite->height)<y){
	// 		continue;
	// 	}
	// 	uint8_t count = min(sprite->width,FRAME_WIDTH-ch->pos_x);
	// 	#if 1
	// 		//8bpp
	// 		uint8_t* data = sprite->data + (y - ch->pos_y)*sprite->width;
	// 		write_buf = pixbuf+ch->pos_x;
	// 		while (count--){
	// 			*(write_buf++)=color_palette[*(data++)];
	// 		}
	// 	#else
	// 		// data converted to 16bpp
	// 		uint16_t* data = sprite->data;
	// 		data +=(y - ch->pos_y)*sprite->width;
	// 		write_buf = pixbuf+ch->pos_x;
	// 		while (count--){
	// 			*(write_buf++)=*(data++);
	// 		}
	// 	#endif
	// }
}

// ----------------------------------------------------------------------------
// DVI setup & launch



uint32_t frame;


uint8_t* _pixelbuffer_location_ptr(uint16_t x,uint16_t y)
{
	assert(active_pixelbuffer!=NULL);
	assert(x>0 || x < active_pixelbuffer->width);
	assert(y>0 || y < active_pixelbuffer->height);

	uint8_t* result = active_pixelbuffer->mem.data;
	result += x + y * active_pixelbuffer->width;
	return result;
}

void gfx_init()
{
	gfx_backend_init();

	gfx_pixelbuffer_create(SEGMENT_GFX_DATA,&initial_pixelbuffer);
	gfx_pixelbuffer_set_active(&initial_pixelbuffer);

	// gfx_renderqueue_add((ng_mem_block_t*)&initial_pixelbuffer);
	// gfx_renderqueue_apply();
}

void gfx_draw()
{
}

void gfx_update()
{
	gfx_backend_update();
}


void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565)
{
	color_palette[color_idx]=color565;
}

void gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx)
{
	gfx_palette_t* palette = assets_get_pointer(asset_id);
	memcpy(color_palette, palette+1, palette->color_amount * sizeof(uint16_t));
	if (palette->color_amount<256){
		uint8_t dif = 256 - palette->color_amount;
		uint16_t fillup_color = color_palette[fill_unused_with_idx];
		uint16_t* tip = color_palette + palette->color_amount;
		while (dif-->0){
			*(tip++) = fillup_color;
		}
	}
}

uint16_t gfx_get_palettecolor(uint8_t color_idx)
{
	return color_palette[color_idx];
}

void     gfx_set_font(uint8_t* font_bpp1)
{
	font = font_bpp1;
}

void     gfx_set_font_from_asset(uint8_t asset_id) 
{
	uint8_t* font = assets_get_pointer(asset_id);
	gfx_set_font(font);
}

// canvas functions
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y)
{
	uint8_t* pixelbuffer_data = _pixelbuffer_location_ptr(x,y);
	return *pixelbuffer_data;
}

void  gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx)
{
	assert(x < active_pixelbuffer->width);
	assert(y < active_pixelbuffer->height);
	uint8_t* pixel = _pixelbuffer_location_ptr(x,y);
	*pixel = color_idx;
}

// get pointer to character in fontbuffer
uint8_t* _char2fontbuffer(uint8_t ch)
{
	uint16_t pos = 0;

	if (ch<127){
		pos = ch-32; 
	}

	return font+pos*8;
}



void gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx)
{
	// for now let's require positive x and y
	if (   x < 0 || x > active_pixelbuffer->width 
		|| y < 0 || y > active_pixelbuffer->height
	){
		return;
	}

	uint8_t* character_ptr = _char2fontbuffer(ch);

	uint8_t width = min(8, active_pixelbuffer->width-x);

	for (int i=0;i<8;i++){
		// set tip on draw-position
		uint8_t* buffer_tip = _pixelbuffer_location_ptr(x,y++);
		uint8_t current_font_line = *(character_ptr++);
		uint8_t mask = 1;
		printf("Print: %c : %d\n",ch);
		// unfold? let's not wait with crazy stuff
		for (int j=0;j<width;j++){
			if (current_font_line & mask){
				*buffer_tip = color_idx;
			} else {
				*buffer_tip = COL_TRANSPARENT;
			}
			mask = mask << 1;
			buffer_tip++;
		}
	}
}

void gfx_draw_text(uint16_t x, uint16_t y, const char* txt, uint8_t color_idx)
{
	uint16_t initial_x = x;
	uint8_t initial_color = color_idx;

	static char number[4]={0,0,0,0};

	for	(int i=0,iEnd=strlen(txt);i<iEnd;i++){
		char ch = txt[i];
		switch (ch){
			case '\n': 
				x = initial_x;
				y += 8; 
				break; 
			case '#':
				if (i+1 < iEnd && txt[i+1]=='#'){
					int pos = i+2;

					if (txt[pos]=='R'){
						color_idx = initial_color;
						i=pos;
						continue;
					} else {
						strncpy(number,txt + pos,3);
						color_idx = atoi(number);
						i=pos+1+1;
						continue;
					}
				}
			default:
				gfx_draw_char(x,y,ch,color_idx);
				x+=8;
				break;
		}
	}
}

void gfx_draw_printf(uint16_t x,uint16_t y,uint8_t color_idx,const char *format, ...) {
	va_list arg;
    va_start(arg, format);
    char temp[64];
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);

    if (len > sizeof(temp) - 1) {
		// start over
        char newtemp[len+1];
        va_start(arg, format);
        vsnprintf(newtemp, sizeof(temp), format, arg);
        va_end(arg);
	    gfx_draw_text(x,y,newtemp, color_idx);
    } else {
		gfx_draw_text(x,y,temp, color_idx);
	}
}

bool gfx_pixelbuffer_create(uint8_t segment_id,gfx_pixelbuffer_t* initial_data)
{
	assert(initial_data->mem.data==NULL && "gfx_create_pixelbuffer: pixelbuffer already initialized!");

	uint32_t size = initial_data->width * initial_data->height;
	assert(ng_mem_segment_space_left(segment_id) > size && "gfx_create_pixelbuffer: create size");
	
	bool success = ng_mem_allocate_block(segment_id, size, MEM_USAGE_PIXELBUFFER, &initial_data->mem );
	return success;
}

void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer)
{
	active_pixelbuffer = pxbuffer;
}

gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void)
{
	return active_pixelbuffer;
}

void gfx_renderqueue_add(ng_mem_block_t* renderblock)
{
	assert(renderqueue_request_amount+1 < GFX_RENDERQUEUE_MAX_ELEMENTS);
	assert(!requested_renderqueue_apply);
	renderqueue_request[renderqueue_request_amount++]=renderblock;
}

void gfx_renderqueue_wipe(void)
{
	for (int i=0;i<GFX_RENDERQUEUE_MAX_ELEMENTS;i++){
		renderqueue_request[i]=NULL;
	}
	renderqueue_request_amount=0;
}

void gfx_renderqueue_apply(void)
{
	requested_renderqueue_apply=true;
	renderqueue_request_amount=0;
}

gfx_tilesheet_t* asset_get_tilesheet(uint8_t asset_id){
	gfx_tilesheet_t* assetdata = assets_get_pointer(asset_id);
	assert(assetdata->type==ASSET_TYPE_TILESHEET && "Tried to get wrong asset-type!");

	gfx_tilesheet_t* tilesheet = ng_mem_allocate(default_allocation_segment,sizeof(gfx_tilesheet_t));
	*tilesheet = *assetdata;
	tilesheet->tile_amount = tilesheet->rows*tilesheet->cols;
	tilesheet->flags=0;
	
	uint32_t data_size = sizeof(uint8_t*)*tilesheet->tile_amount;
	tilesheet->cached_tile_ptrs = ng_mem_allocate(default_allocation_segment,data_size);
	memset(tilesheet->cached_tile_ptrs,0,data_size);

	tilesheet->tilesheet_data_raw = ((uint8_t*)assetdata)+5;

	return tilesheet;
}
