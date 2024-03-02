#include "ng_gfx.h"

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
# include "pico/platform.h"
#endif

#include "gen/color_palette.h"
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

#include <stddef.h>

uint8_t default_allocation_segment = SEGMENT_GFX_DATA;


// // TODO: this must be done somewhere else in userspace
// gfx_pixelbuffer_t initial_pixelbuffer={
// 	.x=0,
// 	.y=0,
// 	.width=320,
// 	.height=100,
// };


// bool __scratch_x("render") requested_renderqueue_apply = false;
// ng_mem_block_t* __scratch_x("render")renderqueue_core1[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
// ng_mem_block_t* __scratch_y("render")renderqueue_request[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
bool requested_renderqueue_apply = false;
ng_mem_block_t* renderqueue_1[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
ng_mem_block_t* renderqueue_2[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};

uint8_t renderqueue_request_amount=0;
ng_mem_block_t** renderqueue_current = renderqueue_1;
ng_mem_block_t** renderqueue_request = renderqueue_2;

const uint8_t* font=NULL; 		  // 1bpp
ng_mem_datablock_t* active_pixelbuffer_data = NULL;
gfx_pixelbuffer_t* active_pixelbuffer = NULL;

uint16_t gfx_color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}


static inline int clip(int x, int min, int max) {
	return x < min ? min : x > max ? max : x;
}

uint16_t* sp16 = NULL;



uint8_t gfx_spritebuffer_create(gfx_sprite_t* spritedata,uint8_t spriteamount) {
	assert(spritedata!=NULL && "spritedata must not be null");
    assert(spriteamount>0 && "you cannot create spritebuffer with amount 0");

	assert(ng_mem_segment_space_left(SEGMENT_GFX_DATA) > sizeof(gfx_internal_spritebuffer_t) && "gfx_spritebuffer_create: exceeded space");

    gfx_internal_spritebuffer_t* spritebuffer = ng_mem_allocate(SEGMENT_GFX_DATA, sizeof(gfx_internal_spritebuffer_t));
	spritebuffer->amount_sprites=spriteamount;
    spritebuffer->sprites = spritedata;
    ng_mem_allocate_block(SEGMENT_GFX_DATA,spriteamount * sizeof(gfx_internal_sprite_t), MEM_USAGE_SPRITEBUFFER, &spritebuffer->mem);
    ng_memblock_wipe(&spritebuffer->mem);
    spritebuffer->sprite_internals=(gfx_internal_sprite_t*)spritebuffer->mem.data;

    uint8_t spritebuffer_id = id_store(spritebuffer);

    uint8_t count = spriteamount;
    for (int idx=0;idx<spriteamount;idx++){
        gfx_sprite_t* sprite_current = &spritebuffer->sprites[idx];
        sprite_current->spritebuffer_id = spritebuffer_id;
        sprite_current->sprite_idx=idx;
    }

    return spritebuffer_id;
}


void _gfx_sprite_set_tileid(gfx_sprite_t* sprite,gfx_internal_sprite_t* sprite_internal, uint8_t tile_idx){
	assert(sprite_internal->tilesheet!=NULL && "no tilesheet set for sprite!");
    assert(tile_idx < sprite_internal->tilesheet->data.tile_amount && "exceeded tile-id");
    
	sprite_internal->tile_ptr = gfx_tilesheet_get_chached_tile(sprite_internal->tilesheet, tile_idx);
	sprite->tile_idx = tile_idx;
}

void gfx_sprite_set_tileset(gfx_sprite_t* sprite, gfx_tilesheet_data_t* tsdata, uint8_t initial_tile_idx) {
    assert(sprite!=NULL);

    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);
    gfx_tilesheet_t* ts = id_get_ptr(tsdata->ts_id);

    gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[sprite->sprite_idx];

    sprite_internal->tilesheet = ts;
    sprite->flags = 1;
    _gfx_sprite_set_tileid(sprite, sprite_internal, initial_tile_idx);
}

void gfx_sprite_set_tileid(gfx_sprite_t* sprite,uint8_t tile_idx){
    assert(sprite!=NULL);
    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);

	if (sprite->tile_idx == tile_idx){
		return;
	}
    gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[sprite->sprite_idx];

    _gfx_sprite_set_tileid(sprite,sprite_internal,tile_idx);
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
	memset(pixbuf,color_palette[32],SCREEN_WIDTH*sizeof(uint16_t)); // make this a renderqueue-command

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

                // TODO: MEMO to myself. This is just a first version, just finish and iterate over it. 
        		ng_mem_datablock_t* db = (ng_mem_datablock_t*)current_render_block;
				gfx_pixelbuffer_t* pixelbuffer = db->info;

                uint8_t px_width;
                uint8_t px_height;
                flags_unpack_4_4(pixelbuffer->pixel_size,px_width,px_height);

				int16_t pixel_y = y - pixelbuffer->y;
				bool is_visible = pixel_y >= 0 && pixel_y < pixelbuffer->height * px_height;
                if (!is_visible) {
                    break;
                }    

                uint16_t full_width = pixelbuffer->width*px_width;
                
                uint16_t output_pixels_to_write;
                uint8_t  output_subpixels_left;

                uint16_t input_pixels_to_read;
                // point to the beginning of the pixelbuffer
                uint8_t* read_buffer = db->mem.data + (pixel_y/px_height)*pixelbuffer->width;
                
                if (pixelbuffer->x>=0){
                    output_pixels_to_write = min(full_width, SCREEN_WIDTH-pixelbuffer->x)-1;
                    output_subpixels_left = px_width;
                    write_buf += pixelbuffer->x; 
                } else {
                    // the pixelbuffer.x is negative
                    output_pixels_to_write = full_width+pixelbuffer->x-1; 
                    output_subpixels_left = px_width + pixelbuffer->x%px_width; // start inbetween a subpixel
                    read_buffer-= pixelbuffer->x/px_width; // move forward(!) to the start-pixel (right side is negative)
                }
                input_pixels_to_read = (output_pixels_to_write / px_width)+1;

                while (input_pixels_to_read--){
                    uint8_t  data  = *(read_buffer++);
                    uint16_t color = color_palette[data];

                    while(output_subpixels_left--){
                        *(write_buf++)=color;
                        if (output_pixels_to_write--==0){
                            goto exit_all_loops;
                        }
                    }
                    output_subpixels_left=px_width;
                }
                
exit_all_loops:
				break;
			}

			case MEM_USAGE_SPRITEBUFFER: {
				// █▀ █▀█ █▀█ █ ▀█▀ █▀▀ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█
				// ▄█ █▀▀ █▀▄ █ ░█░ ██▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄				
				gfx_internal_spritebuffer_t* spritebuffer = (gfx_internal_spritebuffer_t*)current_render_block;

				uint16_t* write_buf = pixbuf;

				uint8_t max_sprites = spritebuffer->amount_sprites;
				gfx_sprite_t* sprite = (gfx_sprite_t*)spritebuffer->sprites;
                gfx_internal_sprite_t* sprite_internals = spritebuffer->sprite_internals;
				while(max_sprites--){
					if (sprite->flags>0){
						gfx_tilesheet_t* ts =  sprite_internals->tilesheet;
						if (sprite->y > y || (sprite->y+ts->data.tile_height)<=y){
							sprite++;
                            sprite_internals++;
							continue;
						}

						uint8_t input_pixels_to_read = min(sprite_internals->tilesheet->data.tile_width,SCREEN_WIDTH-sprite->x);
						uint8_t* data = sprite_internals->tile_ptr + (y - sprite->y)*ts->data.tile_width;
						write_buf = pixbuf+sprite->x;
						uint8_t idx;
						while (input_pixels_to_read--){
							idx = *(data++);
							if (idx==255){
                                //-- DEBUG-TRANSPARENCY---------------
                                //*(write_buf++)=color_palette[COL_RED];
                                //------------------------------------
								write_buf++;
								continue;
							}
							*(write_buf++)=color_palette[idx];
						}		
					}
					sprite++;
                    sprite_internals++;
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

	uint8_t* result = active_pixelbuffer_data->mem.data;
	result += x + y * active_pixelbuffer->width;
	return result;
}

void gfx_init()
{
	gfx_backend_init();

	// gfx_pixelbuffer_create(&initial_pixelbuffer,SEGMENT_GFX_DATA);
	// gfx_pixelbuffer_set_active(&initial_pixelbuffer);

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

void     gfx_set_font(const uint8_t* font_bpp1)
{
	font = font_bpp1;
}

void     gfx_set_font_from_asset(uint8_t asset_id) 
{
	const uint8_t* font = assets_get_pointer(asset_id);
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
	}
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


void gfx_renderqueue_add_id(uint8_t id) {
	ng_mem_datablock_t* datablock = (ng_mem_datablock_t*)id_get_ptr(id);
	assert(datablock!=NULL && "renderqueue element is NULL");
	gfx_renderqueue_add(&datablock->mem);
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

void asset_get_tilesheet(gfx_tilesheet_data_t* tilesheet_data,uint8_t asset_id){
	const gfx_tilesheet_t* assetdata = assets_get_pointer(asset_id);
	assert(assetdata->data.type==ASSET_TYPE_TILESHEET && "Tried to get wrong asset-type!");

	gfx_tilesheet_t* tilesheet = ng_mem_allocate(default_allocation_segment,sizeof(gfx_tilesheet_t));
	
	//*tilesheet = *assetdata;
	memcpy(tilesheet,assetdata,sizeof(gfx_tilesheet_t));

	uint32_t cached_tileptr_size = sizeof(uint8_t*)*tilesheet->data.tile_amount;
	tilesheet->cached_tile_ptrs = ng_mem_allocate(default_allocation_segment,cached_tileptr_size);
	memset(tilesheet->cached_tile_ptrs,0,cached_tileptr_size);

	tilesheet->tilesheet_data_raw = ((uint8_t*)assetdata)+sizeof(gfx_tilesheet_data_t);

    tilesheet->data.ts_id = id_store(tilesheet);
    *tilesheet_data = tilesheet->data;
}

void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination){
	ng_mem_datablock_t* px_datablock = (ng_mem_datablock_t*)id_get_ptr(pxb->obj_id);
    ng_mem_mount_block((ng_mem_block_t*)px_datablock,destination);         
}

void gfx_tilesheet_query_data(uint8_t ts_id,gfx_tilesheet_data_t* data) {
    gfx_tilesheet_data_t* ts = id_get_ptr(ts_id);
    *data=*ts;
}