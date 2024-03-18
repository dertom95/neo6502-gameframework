#include "ng_gfx.h"


// █▀█ █ ▀▄▀ █▀▀ █░░ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█   █▀█ █▀▀ █░░ ▄▀█ ▀█▀ █▀▀ █▀▄
// █▀▀ █ █░█ ██▄ █▄▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄   █▀▄ ██▄ █▄▄ █▀█ ░█░ ██▄ █▄▀

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
# include "pico/platform.h"
#endif

extern uint16_t color_palette[];
//#include "zelda_mini_plus_walk_rgab5515.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "api/gen/ng_api.h"

#include "ng_io.h"
#include <math.h>

#include "ng_utils.h"
#include <assert.h>
#include "core/memory.h"
#include "ng_assets.h"
#include "ng_utils.h"

#include <stddef.h>
#include <stdint.h>

ng_mem_datablock_t* active_pixelbuffer_data = NULL;
gfx_pixelbuffer_t* active_pixelbuffer = NULL;

const uint8_t* font=NULL; 		  // 1bpp

// ---------------
// 🇵​​​​​🇦​​​​​🇱​​​​​🇪​​​​​🇹​​​​​🇹​​​​​🇪​​​​​
// ---------------

void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565)
{
	color_palette[color_idx]=color565;
}

void gfx_set_palette_from_assset(uint8_t asset_id, uint8_t fill_unused_with_idx)
{
	const gfx_palette_t* palette = assets_get_pointer(asset_id);
	memcpy(color_palette, &palette->colors, palette->color_amount * sizeof(uint16_t));
	// for (uint16_t i=0,iEnd=palette->color_amount;i<iEnd;i++){
	// 	color_palette[i]=palette->colors[i];
	// }
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

// ---------
// 🇫​​​​​🇴​​​​​🇳​​​​​🇹​​​​​
// ---------

void     gfx_set_font(const uint8_t* font_bpp1)
{
	font = font_bpp1;
}

void     gfx_set_font_from_asset(uint8_t asset_id) 
{
	const uint8_t* font = assets_get_pointer(asset_id);
	gfx_set_font(font);
}

// -----------------------
// 🇵​​​​​🇮​​​​​🇽​​​​​🇪​​​​​🇱​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​
// -----------------------

uint8_t* _pixelbuffer_location_ptr(uint16_t x,uint16_t y)
{
	assert(active_pixelbuffer!=NULL);
	assert(x>0 || x < active_pixelbuffer->width);
	assert(y>0 || y < active_pixelbuffer->height);

	uint8_t* result = active_pixelbuffer_data->mem.data;
	result += x + y * active_pixelbuffer->width;
	return result;
}

void gfx_pixelbuffer_create(gfx_pixelbuffer_t* initial_data)
{
	assert(initial_data->obj_id==0 && "object-id already set!");
	assert(ng_mem_segment_space_left(SEGMENT_GFX_DATA) > sizeof(ng_mem_datablock_t) && "gfx_create_pixelbuffer: create size");
	
    
    ng_mem_datablock_t* block = ng_mem_allocate(SEGMENT_GFX_DATA, sizeof(ng_mem_datablock_t));
	block->info = initial_data;

	uint32_t size = initial_data->width * initial_data->height;
	assert(ng_mem_segment_space_left(SEGMENT_GFX_DATA) > size && "gfx_create_pixelbuffer: create size");
	
	bool success = ng_mem_allocate_block(SEGMENT_GFX_DATA, size, MEM_USAGE_PIXELBUFFER, &block->mem );
	if (success){
		initial_data->obj_id = id_store(block);

        gfx_pixelbuffer_apply_data(initial_data);
	}
}

void gfx_pixelbuffer_apply_data(gfx_pixelbuffer_t* pixelbuffer)
{
    if (pixelbuffer->x > SCREEN_WIDTH){
        flags_unset(pixelbuffer->flags,PXBFLAG_VISIBLE);
        return;
    }
    
    uint8_t px_width;
    uint8_t px_height;    
    flags_unpack_4_4(pixelbuffer->pixel_size, px_width, px_height);
    
    if (px_width==0 || px_height==0){
        flags_unset(pixelbuffer->flags,PXBFLAG_VISIBLE);
        return;
    }

    uint16_t fw = pixelbuffer->full_width = pixelbuffer->width * px_width;
    if (pixelbuffer->x+fw<0){
        flags_unset(pixelbuffer->flags,PXBFLAG_VISIBLE);
        return;
    }

    flags_set(pixelbuffer->flags,PXBFLAG_VISIBLE);
    
    if (pixelbuffer->x >= 0)
    {
        pixelbuffer->output_pixels_to_write = min(pixelbuffer->full_width, SCREEN_WIDTH - pixelbuffer->x) - 1;
        pixelbuffer->output_subpixels_start = px_width;
        pixelbuffer->writebuf_offset = pixelbuffer->x;
        pixelbuffer->readbuf_offset = 0;
        pixelbuffer->input_pixels_to_read = (pixelbuffer->output_pixels_to_write / px_width);    
    }
    else
    {
        // the pixelbuffer.x is negative
        pixelbuffer->output_subpixels_start = px_width + pixelbuffer->x % px_width; // start inbetween a subpixel

        pixelbuffer->output_pixels_to_write = min(pixelbuffer->full_width + pixelbuffer->x,SCREEN_WIDTH) - 1;
        pixelbuffer->readbuf_offset = -pixelbuffer->x / px_width;                    
        pixelbuffer->writebuf_offset = 0;
        pixelbuffer->input_pixels_to_read = (pixelbuffer->output_pixels_to_write / px_width);    
    }

    printf("px:%d input_pixels_to_read:%d roffset:%d\n",pixelbuffer->x,pixelbuffer->input_pixels_to_read,pixelbuffer->readbuf_offset);

    flags_unset(pixelbuffer->flags,PXBFLAG_DIRTY);
}

void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer)
{
	assert(pxbuffer->obj_id!=0 && "pixelbuffer not initialized");
	ng_mem_datablock_t* px_datablock = id_get_ptr(pxbuffer->obj_id);
	active_pixelbuffer_data = px_datablock;
	active_pixelbuffer = pxbuffer;
}

gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void) {
	assert(active_pixelbuffer!=NULL && "Active Pixelbuffer not set!");
	return active_pixelbuffer;
}

void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination){
	ng_mem_datablock_t* px_datablock = (ng_mem_datablock_t*)id_get_ptr(pxb->obj_id);
    ng_mem_mount_block((ng_mem_block_t*)px_datablock,destination);         
}


// ------------------------------------
// 🇵​​​​​🇽​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​ - 🇬​​​​​🇷​​​​​🇦​​​​​🇵​​​​​🇭​​​​​🇮​​​​​🇨​​​​​🇸​​​​​
// ------------------------------------

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
const uint8_t* _char2fontbuffer(uint8_t ch)
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

	const uint8_t* character_ptr = _char2fontbuffer(ch);

	uint8_t width = min(8, active_pixelbuffer->width-x);

	for (int i=0;i<8;i++){
		// set tip on draw-position
		uint8_t* buffer_tip = _pixelbuffer_location_ptr(x,y++);
		uint8_t current_font_line = *(character_ptr++);
		uint8_t mask = 1;
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

void gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx)
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

