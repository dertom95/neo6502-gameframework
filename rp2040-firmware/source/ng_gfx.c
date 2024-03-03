#include "ng_gfx.h"


// █▀▀ █▀▀ ▀▄▀   █▀▀ █▀▀ █▄░█ █▀▀ █▀█ ▄▀█ █░░
// █▄█ █▀░ █░█   █▄█ ██▄ █░▀█ ██▄ █▀▄ █▀█ █▄▄

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
#include "ng_utils.h"

#include <stddef.h>

uint8_t default_allocation_segment = SEGMENT_GFX_DATA;

bool requested_renderqueue_apply = false;
ng_mem_block_t* renderqueue_1[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};
ng_mem_block_t* renderqueue_2[GFX_RENDERQUEUE_MAX_ELEMENTS]={0};

uint8_t renderqueue_request_amount=0;
ng_mem_block_t** renderqueue_current = renderqueue_1;
ng_mem_block_t** renderqueue_request = renderqueue_2;

uint16_t frame;

uint16_t gfx_color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

static inline int clip(int x, int min, int max) {
	return x < min ? min : x > max ? max : x;
}


void gfx_init()
{
	gfx_backend_init();
}

void gfx_draw()
{
}

void gfx_update()
{
	gfx_backend_update();
}

// ------------------------
// 🇷​​​​​🇪​​​​​🇳​​​​​🇩​​​​​🇪​​​​​🇷 ​​​​​🇶​​​​​🇺​​​​​🇪​​​​​🇺​​​​​🇪​​​​​
// ------------------------

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
                gfx_internal_sprite_t* sprite_internal = spritebuffer->sprite_internals;
                
				while(max_sprites--){
                    if (sprite->flags==0){
                        sprite++;
                        sprite_internal++;
                        continue;
                    }

                    gfx_tilesheet_t* ts =  sprite_internal->tilesheet;

                    uint8_t px_width;
                    uint8_t px_height;
                    flags_unpack_4_4(sprite->pixel_size,px_width,px_height);                    

                    uint16_t sprite_height = ts->data.tile_height * px_height;

                    int16_t sprite_y = sprite->y;

                    uint8_t alignment_v = flags_mask_value(sprite->flags,SPRITEFLAG_ALIGNV_MASK);
                    if (alignment_v==SPRITEFLAG_ALIGNV_CENTER){
                        sprite_y -= ts->data.tile_height_half * px_height;
                    }
                    else if(alignment_v==SPRITEFLAG_ALIGNV_BOTTOM){
                        sprite_y -= sprite_height;
                    }

                    if (sprite_y > y || (sprite_y+sprite_height)<=y){
                        sprite++;
                        sprite_internal++;
                        continue;
                    }

                    bool flipped_v = flags_isset(sprite->flags,SPRITEFLAG_FLIP_V);
                    bool flipped_h = flags_isset(sprite->flags,SPRITEFLAG_FLIP_H);

                    uint16_t sprite_width = ts->data.tile_width * px_width;

                    uint8_t alignment_h = flags_mask_value(sprite->flags,SPRITEFLAG_ALIGNH_MASK);
                    int16_t sprite_x = sprite->x;                    
                    int8_t read_direction= flipped_h ? -1 : 1;

                    if (alignment_h==SPRITEFLAG_ALIGNH_CENTER){
                        sprite_x -= ts->data.tile_width_half * px_width;
                    }
                    else if(alignment_h==SPRITEFLAG_ALIGNH_RIGHT){
                        sprite_x -= sprite_width;
                    }


                    uint8_t line = flipped_v ? sprite_height - (y - sprite_y) / px_height -1
                                                : (y - sprite_y) / px_height;

                    // apply pixel-height

                    uint8_t input_pixels_to_read = min(ts->data.tile_width,SCREEN_WIDTH-sprite_x);

                    uint8_t* data = sprite_internal->tile_ptr + line * ts->data.tile_width;

                    int16_t output_pixels_to_write;
                    int8_t subpixel_left_to_write;
                    if (sprite_x >= 0){
                        subpixel_left_to_write = px_width;
                        write_buf = pixbuf+sprite_x;
                        output_pixels_to_write = min(sprite_width, SCREEN_WIDTH-sprite_x);
                    } else {
                        subpixel_left_to_write = px_width + sprite_x % px_width;
                        write_buf = pixbuf;
                        output_pixels_to_write = sprite_width + sprite_x - 1;
                        data += -sprite_x/px_width;
                    }


                    if (flipped_h){
                        // TODO: fix this
                        data += ts->data.tile_width - 1;
                    }

                    uint8_t idx;
                    while (input_pixels_to_read--){
                        idx = *(data);
                        data += read_direction;
                        uint16_t color = color_palette[idx];
                        while (subpixel_left_to_write--){
                            if (idx==255){
                                //-- DEBUG-TRANSPARENCY---------------
                                //color = color_palette[COL_RED];
                                //------------------------------------
                                write_buf++;
                            } else {
                                *(write_buf++)=color;
                            }

                            if (output_pixels_to_write--==0){
                                goto break_out;
                            }
                        }
                        subpixel_left_to_write = px_width;
                    }		
break_out:					
					sprite++;
                    sprite_internal++;
				}
				break;
			}
		}

	}
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

