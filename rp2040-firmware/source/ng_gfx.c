#include "ng_gfx.h"

// █▀▀ █▀▀ ▀▄▀   █▀▀ █▀▀ █▄░█ █▀▀ █▀█ ▄▀█ █░░
// █▄█ █▀░ █░█   █▄█ ██▄ █░▀█ ██▄ █▀▄ █▀█ █▄▄

#ifdef PICO_NEO6502
#include "core/backend/neo6502/neo6502.h"
#include "pico/platform.h"
#endif

// #include "zelda_mini_plus_walk_rgab5515.h"
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
ng_mem_block_t *renderqueue_1[GFX_RENDERQUEUE_MAX_ELEMENTS] = {0};
ng_mem_block_t *renderqueue_2[GFX_RENDERQUEUE_MAX_ELEMENTS] = {0};

uint8_t renderqueue_request_amount = 0;
ng_mem_block_t **renderqueue_current = renderqueue_1;
ng_mem_block_t **renderqueue_request = renderqueue_2;

uint16_t color_palette[] = {
    0x0000,
    0x0000,
    0x194A,
    0x792A,
    0x042A,
    0xAA86,
    0x5AA9,
    0xC618,
    0xFF9D,
    0xF800,
    0xFD00,
    0xFF64,
    0x0726,
    0x2D7F,
    0x83B3,
    0xFBB5,
    0xFE75,
    0x71E7,
    0x8A88,
    0x9B09,
    0xAB6A,
    0xD46D,
    0xFD50,
    0xFE6F,
    0x0126,
    0x01CA,
    0x026B,
    0x0B2F,
    0x0371,
    0x3474,
    0x257A,
    0x8EBF,
    0x6145,
    0x91A7,
    0xB268,
    0xCAE8,
    0xE3C8,
    0xFCC9,
    0xFDE9,
    0xFF49,
    0x2949,
    0x3A2D,
    0x62F0,
    0x7BB3,
    0x8496,
    0x959B,
    0xC6BF,
    0xC77F,
    0x0103,
    0x0184,
    0x1243,
    0x22C3,
    0x2B41,
    0x5444,
    0x7D25,
    0xA666,
    0x18E5,
    0x2189,
    0x222D,
    0x3351,
    0x3477,
    0x459C,
    0x569F,
    0x77BF,
    0x1985,
    0x29E7,
    0x3A88,
    0x32E8,
    0x43AA,
    0x4C4C,
    0x55AF,
    0x96D4,
    0x5822,
    0x8103,
    0xB1E6,
    0xE2EB,
    0xFBAE,
    0xFCD5,
    0xFDD8,
    0xFEDF,
    0x2986,
    0x4A29,
    0x5AED,
    0x738F,
    0x8432,
    0xAD77,
    0xBE3B,
    0xEF9E,
    0x3987,
    0x59E8,
    0x8A8B,
    0xAB4C,
    0xC40D,
    0xDCEE,
    0xEE30,
    0xFFD5,
    0x3105,
    0x49A7,
    0x5A28,
    0x72CA,
    0x7B6A,
    0x9C4D,
    0xC531,
    0xDDF3,
    0x2884,
    0x4947,
    0x61AB,
    0x92AE,
    0xBB72,
    0xC3D5,
    0xDCD7,
    0xFE3B,
    0x0169,
    0x020A,
    0x028C,
    0x034D,
    0x040F,
    0x0510,
    0x05F4,
    0x06FB,
    0x4184,
    0x6247,
    0x7B08,
    0x9BCA,
    0xB48C,
    0xCD4D,
    0xEE50,
    0xFF54,
    0x5844,
    0x6926,
    0x9228,
    0xA30A,
    0xBBEC,
    0xCCAE,
    0xEDAF,
    0xEEB2,
    0x31AB,
    0x4A90,
    0x6333,
    0x7BB8,
    0x8C7C,
    0x9CDD,
    0xBD7F,
    0xDEBF,
    0x40A5,
    0x7147,
    0x99CA,
    0xDA4D,
    0xFAF0,
    0xFBF5,
    0xFD38,
    0xFE7F,
    0x4923,
    0x61A6,
    0x7A48,
    0x9ACB,
    0xAB6D,
    0xC3EF,
    0xD46F,
    0xE4CF,
    0x2140,
    0x2A61,
    0x4AE0,
    0x6381,
    0x7C03,
    0x94C4,
    0xB546,
    0xD666,
    0x6140,
    0x71C1,
    0x8262,
    0x9B24,
    0xBC45,
    0xD547,
    0xEE89,
    0xFFA9,
    0x2107,
    0x39CA,
    0x528D,
    0x734D,
    0x93CF,
    0xB4B0,
    0xCD71,
    0xFEF6,
    0x1968,
    0x29E8,
    0x3A67,
    0x4AE6,
    0x5B85,
    0x6C25,
    0x7CE4,
    0x7DE7,
    0x3124,
    0x51C7,
    0x7A69,
    0x92E9,
    0xAB6B,
    0xBBEC,
    0xD48E,
    0xF510,
    0x2A48,
    0x432B,
    0x5BCD,
    0x74AF,
    0x8571,
    0x8E12,
    0xAE98,
    0xE7DD,
    0x00C8,
    0x018B,
    0x024F,
    0x12F4,
    0x13B8,
    0x44BD,
    0x559E,
    0x6E7F,
    0x522D,
    0x72EE,
    0x93B1,
    0xBC72,
    0xD513,
    0xEDF3,
    0xFEB3,
    0xFFB0,
    0x18E4,
    0x398A,
    0x5A4F,
    0x7B37,
    0x943E,
    0xACBD,
    0xBD5E,
    0xD5FF,
    0x212A,
    0x298B,
    0x29EE,
    0x3A90,
    0x5335,
    0x53B8,
    0x6C18,
    0x8498,
    0x4905,
    0x5A09,
    0x728B,
    0x930D,
    0xABD0,
    0xB452,
    0xD575,
    0xFF19,
    0x70E0,
    0x9984,
    0xBAC7,
    0xEC24,
    0xFD81,
    0xFE60,
    0xFF85,
    0xF7B7,
};
uint16_t frame;

uint16_t gfx_color565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

static inline int clip(int x, int min, int max)
{
    return x < min ? min : x > max ? max
                                   : x;
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
    uint16_t* end = pixbuf + SCREEN_WIDTH_HALF;
    //memset(pixbuf,color_palette[32],SCREEN_WIDTH*sizeof(uint16_t)); // make this a renderqueue-command
    memset(pixbuf,0,SCREEN_WIDTH_HALF *sizeof(uint32_t)); // make this a renderqueue-command
    
    for (uint8_t idx = 0; idx < GFX_RENDERQUEUE_MAX_ELEMENTS; idx++)
    {
        ng_mem_block_t *current_render_block = renderqueue_current[idx];
        if (current_render_block == NULL)
        {
            break;
        }

        uint8_t usage = ng_memblock_get_usage(current_render_block);

        uint16_t *write_buf = pixbuf;

        switch (usage)
        {
        case MEM_USAGE_PIXELBUFFER:
        {
            // █▀█ █ ▀▄▀ █▀▀ █░░ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█
            // █▀▀ █ █░█ ██▄ █▄▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄

            // TODO: MEMO to myself. This is just a first version, just finish and iterate over it.
            ng_mem_datablock_t *db = (ng_mem_datablock_t *)current_render_block;
            gfx_pixelbuffer_t *pixelbuffer = db->info;

            if (flags_isset(pixelbuffer->flags,PXBFLAG_DIRTY)){
                gfx_pixelbuffer_apply_data(pixelbuffer);
            }

            uint8_t px_width;
            uint8_t px_height;
            flags_unpack_4_4(pixelbuffer->pixel_size, px_width, px_height);

            int16_t pixel_y = y - pixelbuffer->y;
            bool is_visible = pixel_y >= 0 && pixel_y < pixelbuffer->height * px_height;
            if (!is_visible)
            {
                break;
            }

            if (px_width == 1 && px_height == 1 && pixelbuffer->width == SCREEN_WIDTH)
            {
                // █▀ ▀█▀ ▄▀█ █▄░█ █▀▄ ▄▀█ █▀█ █▀▄
                // ▄█ ░█░ █▀█ █░▀█ █▄▀ █▀█ █▀▄ █▄▀                
                uint8_t *readbuffer = db->mem.data + pixel_y * pixelbuffer->width;
                uint16_t counter = SCREEN_WIDTH;
                uint8_t last_index=0;
                uint16_t last_color=color_palette[0];
                while (counter--)
                {
                    uint8_t new_idx = *(readbuffer++);
                    if (new_idx!=last_index){
                        last_color=color_palette[new_idx];
                        last_index=new_idx;
                    }
                    *(write_buf++)=last_color;
                }
            } else {
                uint8_t output_subpixels_start = pixelbuffer->output_subpixels_start;

                uint16_t input_pixels_to_read = pixelbuffer->input_pixels_to_read;
                // point to the beginning of the pixelbuffer
                // TODO: divison by bitshifting only for pow2
                uint8_t *read_buffer = db->mem.data + (pixel_y >> (px_height-1)) * pixelbuffer->width;
                read_buffer += pixelbuffer->readbuf_offset;
                write_buf += pixelbuffer->writebuf_offset;

                uint8_t last_idx = *(read_buffer++);
                uint16_t color = color_palette[last_idx];

                // first write subpixels (only scenario having px-size>1 and x-pos negative)
                switch(output_subpixels_start){
                    case 8: *(write_buf++)=color;
                    case 7: *(write_buf++)=color;
                    case 6: *(write_buf++)=color;
                    case 5: *(write_buf++)=color;
                    case 4: *(write_buf++)=color;
                    case 3: *(write_buf++)=color;
                    case 2: *(write_buf++)=color;
                    case 1: *(write_buf++)=color;
                }
                // I know, I know looks totally silly but it produces the fastest code. 
                if (px_width==8){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                    }                
                }
                else if (px_width==7){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                    }                
                }
                else if (px_width==6){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                    }                
                }
                else if (px_width==5){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        *(write_buf++)=color;*(write_buf++)=color;
                    }                
                }
                else if (px_width==4){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;*(write_buf++)=color;
                        *(write_buf++)=color;*(write_buf++)=color;
                    }                
                }
                else if (px_width==3){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;*(write_buf++)=color;
                        *(write_buf++)=color;
                    }                
                }
                else if (px_width==2){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;
                        *(write_buf++)=color;
                    }                
                }
                else if (px_width==1){
                    while (input_pixels_to_read--)
                    {
                        uint8_t data = *(read_buffer++);
                        if (data!=last_idx){
                            last_idx = data;
                            color = color_palette[data];
                        }
                        *(write_buf++)=color;
                    }                
                }

            }
            break;
        }

        case MEM_USAGE_SPRITEBUFFER:
        {
            // █▀ █▀█ █▀█ █ ▀█▀ █▀▀ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█
            // ▄█ █▀▀ █▀▄ █ ░█░ ██▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄
            gfx_internal_spritebuffer_t *spritebuffer = (gfx_internal_spritebuffer_t *)current_render_block;

            uint16_t *write_buf = pixbuf;

            uint8_t max_sprites = spritebuffer->amount_sprites;
            gfx_sprite_t *sprite = (gfx_sprite_t *)spritebuffer->sprites;
            gfx_internal_sprite_t *si = spritebuffer->sprite_internals;

            while (max_sprites--)
            {
                
                if (flags_isset(sprite->flags,SPRITEFLAG_DIRTY)){
                    gfx_sprite_apply_data(sprite);
                }   

                if (!flags_isset(sprite->flags,SPRITEFLAG_READY|SPRITEFLAG_VISIBLE))
                {
                    sprite++;
                    si++;
                    continue;
                }
             
                gfx_tilesheet_t *ts = si->tilesheet;

                uint8_t px_width;
                uint8_t px_height;
                flags_unpack_4_4(sprite->pixel_size, px_width, px_height);

                //if (px_height == 1 && px_width==1)
                if (false)
                {
                //     // █▀ ▀█▀ ▄▀█ █▄░█ █▀▄ ▄▀█ █▀█ █▀▄   █▀ █▀█ █▀█ █ ▀█▀ █▀▀
                //     // ▄█ ░█░ █▀█ █░▀█ █▄▀ █▀█ █▀▄ █▄▀   ▄█ █▀▀ █▀▄ █ ░█░ ██▄                    

                //     uint8_t *tile_ptr = si->tile_ptr;

                //     uint8_t format = flags_mask_value(ts->data.type, ASSET_TYPE_FILEFORMAT_MASK);
                //     uint8_t offset_left = 0;
                //     uint8_t offset_top = 0;
                //     uint8_t offset_right = 0;
                //     uint8_t offset_bottom = 0;
                //     uint8_t offset_width = ts->data.tile_width;
                //     uint8_t offset_height = ts->data.tile_height;
                //     if (format == ASSET_TYPE_FILEFORMAT_2)
                //     {
                //         uint8_t oX = *(tile_ptr++);
                //         uint8_t oY = *tile_ptr++;
                //         offset_width = *tile_ptr++;
                //         offset_height = *tile_ptr++;

                //         offset_left = oX;
                //         offset_top = oY;
                //         offset_right = ts->data.tile_width - oX - offset_width;
                //         offset_bottom = ts->data.tile_height - oY - offset_height;
                //     }  
                //     uint16_t sprite_height = ts->data.tile_height;

                //     int16_t sprite_y = sprite->y;

                //     uint8_t alignment_v = flags_mask_value(sprite->flags, SPRITEFLAG_ALIGNV_MASK);
                //     if (alignment_v == SPRITEFLAG_ALIGNV_CENTER)
                //     {
                //         sprite_y -= ts->data.tile_height_half;
                //     }
                //     else if (alignment_v == SPRITEFLAG_ALIGNV_BOTTOM)
                //     {
                //         sprite_y -= sprite_height;
                //     }                    
                //     if ((sprite_y + offset_top) > y || (sprite_y + sprite_height - offset_bottom + offset_top) <= y)
                //     {
                //         // no intersection! NEXT SPRITE, PLEASE!
                //         sprite++;
                //         si++;
                //         continue;
                //     }                                      

                //     bool flipped_v = flags_isset(sprite->flags, SPRITEFLAG_FLIP_V);
                //     bool flipped_h = flags_isset(sprite->flags, SPRITEFLAG_FLIP_H);

                //     uint16_t sprite_width = ts->data.tile_width;

                //     uint8_t alignment_h = flags_mask_value(sprite->flags, SPRITEFLAG_ALIGNH_MASK);
                //     int16_t sprite_x = sprite->x;
                //     int8_t read_direction = flipped_h ? -1 : 1;

                //     if (alignment_h == SPRITEFLAG_ALIGNH_CENTER)
                //     {
                //         sprite_x -= ts->data.tile_width_half;
                //     }
                //     else if (alignment_h == SPRITEFLAG_ALIGNH_RIGHT)
                //     {
                //         sprite_x -= sprite_width;
                //     }

                //     uint8_t line = flipped_v ? (offset_height - (y - sprite_y - offset_top) - 1) 
                //                             : (y - sprite_y - offset_top);

                //     // apply pixel-height

                //     uint8_t input_pixels_to_read = min(offset_width, SCREEN_WIDTH - sprite_x);

                //     uint8_t *data = tile_ptr + line * offset_width;

                //     int16_t output_pixels_to_write;

                //     if (flipped_h)
                //     {
                //         data += offset_width - 1;
                //     }

                //     if (sprite_x >= 0)
                //     {
                //         write_buf = pixbuf + sprite_x + offset_left;
                //         output_pixels_to_write = min(offset_width, SCREEN_WIDTH - sprite_x); // TODO: Right-Border using offset-values
                //     }
                //     else
                //     {
                //         write_buf = pixbuf;
                //         output_pixels_to_write = offset_width + sprite_x - 1;
                //         data += sprite_x;
                //     }

                //     uint8_t last_idx=0;
                //     uint16_t color = color_palette[0];

                //     while (input_pixels_to_read--)
                //     {
                //         uint8_t idx = *(data);
                //         data += read_direction;
                //         if (idx == 255){
                //             write_buf++;
                //             continue;
                //         }
                //         if (idx != last_idx){
                //             last_idx = idx;
                //             color = color_palette[last_idx];
                //         }
                //         *(write_buf++) = color;
                //         if (output_pixels_to_write-- == 0)
                //         {
                //             goto break_out2;
                //         }
                //     }
                // break_out2:
                //     sprite++;
                //     si++;

                } else {
                    // █▀ █ ▀█ █▀▀ ▄▀█ █▄▄ █░░ █▀▀   █▀ █▀█ █▀█ █ ▀█▀ █▀▀
                    // ▄█ █ █▄ ██▄ █▀█ █▄█ █▄▄ ██▄   ▄█ █▀▀ █▀▄ █ ░█░ ██▄

                    uint8_t *tile_ptr = si->tile_ptr;

                    if (si->format == ASSET_TYPE_FILEFORMAT_2)
                    {
                        tile_ptr += 4;
                    }

                    if ((si->sprite_y + si->offset_top * px_height) > y || (si->sprite_y + si->sprite_height - si->offset_bottom * px_height) <= y)
                    {
                        sprite++;
                        si++;
                        continue;
                    }

                    int8_t read_direction = si->read_direction;

                    bool flipped_v = flags_isset(sprite->flags, SPRITEFLAG_FLIP_V);

                    uint8_t line = flipped_v ? (si->offset_height - (y - si->sprite_y - si->offset_top * px_height) - 1) / px_height
                                            : (y - si->sprite_y - si->offset_top * px_height) / px_height;

                    // apply pixel-height

                    uint8_t input_pixels_to_read = si->input_pixels_to_read;

                    uint8_t *data = tile_ptr + line * si->offset_width;
                    data += si->readbuf_offset;

                    write_buf = pixbuf + si->writebuf_offset;

                    uint8_t last_idx = *(data);
                    data += read_direction;
                    uint16_t color = color_palette[last_idx];

                    if (last_idx != 255){
                        switch(si->subpixel_left){
                            case 8: *(write_buf++)=color;
                            case 7: *(write_buf++)=color;
                            case 6: *(write_buf++)=color;
                            case 5: *(write_buf++)=color;
                            case 4: *(write_buf++)=color;
                            case 3: *(write_buf++)=color;
                            case 2: *(write_buf++)=color;
                            case 1: *(write_buf++)=color;
                        }
                    } else {
#ifndef DEBUG_TRANSPARENCY                        
                        write_buf+=si->subpixel_left;
#else                        
                        switch(si->subpixel_left){
                            case 8: *(write_buf++)=color_palette[COL_RED];
                            case 7: *(write_buf++)=color_palette[COL_RED];
                            case 6: *(write_buf++)=color_palette[COL_RED];
                            case 5: *(write_buf++)=color_palette[COL_RED];
                            case 4: *(write_buf++)=color_palette[COL_RED];
                            case 3: *(write_buf++)=color_palette[COL_RED];
                            case 2: *(write_buf++)=color_palette[COL_RED];
                            case 1: *(write_buf++)=color_palette[COL_RED];
                        }
#endif                        
                    }


// ------ DEFINE ------------------------------------------------------
#ifndef DEBUG_TRANSPARENCY
# define DEFAULT_LOOP_LOGIC \
                            idx = *(data); \
                            if (idx!=last_idx){ \
                                last_idx = idx; \
                                color = color_palette[idx]; \
                            } \
                            data += read_direction; \
                            if (idx==255) { \
                                write_buf += px_width; \
                                continue; \
                            } \
                            uint16_t color = color_palette[idx]; 
#else 
# define DEFAULT_LOOP_LOGIC \
                            idx = *(data); \
                            if (idx!=last_idx){ \
                                last_idx = idx; \
                                color = color_palette[idx]; \
                            } \
                            data += read_direction; \
                            if (idx==255) { \
                                idx = COL_RED; \
                            } \
                            uint16_t color = color_palette[idx]; 
#endif
// -------------------------------------------------------------------
                    uint8_t idx;
                    if (px_width==1){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;
                        }
                    }
                    else if (px_width==2){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;
                        }
                    }
                    else if (px_width==3){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        }
                    }
                    else if (px_width==4){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        }
                    }
                    else if (px_width==5){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                            *(write_buf++)=color;
                        }
                    }
                    else if (px_width==6){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                            *(write_buf++)=color;*(write_buf++)=color;
                        }
                    }
                    else if (px_width==7){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        }
                    }
                    else if (px_width==8){
                        while (input_pixels_to_read--)
                        {
                            DEFAULT_LOOP_LOGIC
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                            *(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;*(write_buf++)=color;
                        }
                    }

                }
                if ((uintptr_t)write_buf>(uintptr_t)end){
                    int a=0;
                }
                sprite++;
                si++;
                    
            }



            break;
        }
        }
    }
}

void gfx_renderqueue_add_id(uint8_t id)
{
    ng_mem_datablock_t *datablock = (ng_mem_datablock_t *)id_get_ptr(id);
    assert(datablock != NULL && "renderqueue element is NULL");
    gfx_renderqueue_add(&datablock->mem);
}

void gfx_renderqueue_add(ng_mem_block_t *renderblock)
{
    assert(renderqueue_request_amount + 1 < GFX_RENDERQUEUE_MAX_ELEMENTS);
    assert(!requested_renderqueue_apply);
    renderqueue_request[renderqueue_request_amount++] = renderblock;
}

void gfx_renderqueue_wipe(void)
{
    for (uint8_t i = 0; i < GFX_RENDERQUEUE_MAX_ELEMENTS; i++)
    {
        renderqueue_request[i] = NULL;
    }
    renderqueue_request_amount = 0;
}

void gfx_renderqueue_apply(void)
{
    requested_renderqueue_apply = true;
    renderqueue_request_amount = 0;
}
