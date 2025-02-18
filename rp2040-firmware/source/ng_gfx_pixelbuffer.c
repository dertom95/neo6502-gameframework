#include "ng_gfx.h"


// █▀█ █ ▀▄▀ █▀▀ █░░ █▄▄ █░█ █▀▀ █▀▀ █▀▀ █▀█   █▀█ █▀▀ █░░ ▄▀█ ▀█▀ █▀▀ █▀▄
// █▀▀ █ █░█ ██▄ █▄▄ █▄█ █▄█ █▀░ █▀░ ██▄ █▀▄   █▀▄ ██▄ █▄▄ █▀█ ░█░ ██▄ █▄▀

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
//#include "pico/platform.h"
#include "boards/pico.h"
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

char temp_txt_buf[40]={0};

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
	assert(initial_data->obj_id==0 && "pixelbuffer already created[object-id already set]");
	assert(ng_mem_segment_space_left(SEGMENT_GFX_DATA) > sizeof(ng_mem_datablock_t) && "gfx_create_pixelbuffer: create size");
	
    
    ng_mem_datablock_t* block = ng_mem_allocate(SEGMENT_GFX_DATA, sizeof(ng_mem_datablock_t));
	block->info = initial_data;

	uint32_t size = initial_data->width * initial_data->height;
	assert(ng_mem_segment_space_left(SEGMENT_GFX_DATA) > size && "gfx_create_pixelbuffer: create size");
	
	bool success = ng_mem_allocate_block(SEGMENT_GFX_DATA, size, MEM_USAGE_PIXELBUFFER, &block->mem );
	assert(success && "not enough space");
    if (success){
        ng_memblock_wipe(&block->mem,COL_TRANSPARENT);
		initial_data->obj_id = id_store(block);

        gfx_pixelbuffer_apply_data(initial_data);
	}
}

void gfx_pixelbuffer_wipe(gfx_pixelbuffer_t* pxb, uint8_t wipe_value)
{
    ng_mem_block_t* px_datablock = id_get_ptr(pxb->obj_id);
    ng_memblock_wipe(px_datablock,wipe_value);
}

void __not_in_flash_func(gfx_pixelbuffer_apply_data)(gfx_pixelbuffer_t* pixelbuffer)
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
//        pixelbuffer->output_pixels_to_write = min(pixelbuffer->full_width, SCREEN_WIDTH - pixelbuffer->x) - 1;
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

    //printf("px:%d input_pixels_to_read:%d roffset:%d\n",pixelbuffer->x,pixelbuffer->input_pixels_to_read,pixelbuffer->readbuf_offset);

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

void gfx_pixelbuffer_mount(gfx_pixelbuffer_t* pxb, uint16_t destination, uint16_t page_size){
	ng_mem_datablock_t* px_datablock = (ng_mem_datablock_t*)id_get_ptr(pxb->obj_id);
    uint8_t mount_id = ng_mem_mount_block((ng_mem_block_t*)px_datablock,destination, page_size);   
    pxb->mount_id = mount_id;      
}

bool gfx_mount_set_page(uint8_t mount_id, uint8_t page)
{
    bool result = ng_mem_set_datamount_page(mount_id,page);
    return result;
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
    if (!active_pixelbuffer){
        return;
    }
    assert(x < active_pixelbuffer->width);
	assert(y < active_pixelbuffer->height);
	uint8_t* pixel = _pixelbuffer_location_ptr(x,y);
	*pixel = color_idx;
}

// get pointer to character in fontbuffer
const uint8_t* _char2fontbuffer(uint8_t ch)
{
	assert(font!=NULL || "no font loaded");
    uint16_t pos = 0;

	if (ch<127){
		pos = ch-32; 
	}

	return font+pos*8;
}
uint16_t clip_x=0;
uint16_t clip_y=0;
uint16_t clip_w=65535;
uint16_t clip_h=65535;

void    gfx_clip_rect(uint16_t x,uint16_t y, uint16_t w,uint16_t h) {
    clip_x=x;
    clip_y=y;
    clip_w=w;
    clip_h=h;
}

void    gfx_clip_clear() {
    clip_x=0;
    clip_y=0;
    clip_w=65535;
    clip_h=65535;
}



void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color, uint8_t flags) {
    // Calculate the effective bounds of the rectangle after clipping
    uint16_t x_start = (x < clip_x) ? clip_x : x;
    uint16_t y_start = (y < clip_y) ? clip_y : y;
    uint16_t x_end = (x + width > clip_x + clip_w) ? clip_x + clip_w : x + width;
    uint16_t y_end = (y + height > clip_y + clip_h) ? clip_y + clip_h : y + height;

    // If the rectangle is completely outside the clip area, return early
    if (x_start >= x_end || y_start >= y_end) {
        return;
    }

    if (flags & GFX_FLAG_FILLED) {
        // Draw a filled rectangle within the clipped bounds
        for (uint16_t i = y_start; i < y_end; i++) {
            for (uint16_t j = x_start; j < x_end; j++) {
                gfx_draw_pixel(j, i, color);
            }
        }
    } else {
        // Draw only the rectangle outline within the clipped bounds
        // Top edge
        for (uint16_t i = x_start; i < x_end; i++) {
            if (y >= clip_y && y < clip_y + clip_h) {
                gfx_draw_pixel(i, y, color);
            }
        }
        // Bottom edge
        for (uint16_t i = x_start; i < x_end; i++) {
            if (y + height - 1 >= clip_y && y + height - 1 < clip_y + clip_h) {
                gfx_draw_pixel(i, y + height - 1, color);
            }
        }
        // Left edge
        for (uint16_t i = y_start; i < y_end; i++) {
            if (x >= clip_x && x < clip_x + clip_w) {
                gfx_draw_pixel(x, i, color);
            }
        }
        // Right edge
        for (uint16_t i = y_start; i < y_end; i++) {
            if (x + width - 1 >= clip_x && x + width - 1 < clip_x + clip_w) {
                gfx_draw_pixel(x + width - 1, i, color);
            }
        }
    }
}


void gfx_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color, uint8_t flags) {
    int16_t dx = 0;
    int16_t dy = radius;
    int16_t d = 1 - radius;

    if (flags & GFX_FLAG_FILLED) {
        // Draw a filled circle
        while (dx <= dy) {
            for (int16_t i = x - dx; i <= x + dx; i++) {
                gfx_draw_pixel(i, y + dy, color);
                gfx_draw_pixel(i, y - dy, color);
            }
            for (int16_t i = x - dy; i <= x + dy; i++) {
                gfx_draw_pixel(i, y + dx, color);
                gfx_draw_pixel(i, y - dx, color);
            }
            if (d < 0) {
                d += 2 * dx + 3;
            } else {
                d += 2 * (dx - dy) + 5;
                dy--;
            }
            dx++;
        }
    } else {
        // Draw only the circle outline
        while (dx <= dy) {
            gfx_draw_pixel(x + dx, y + dy, color);
            gfx_draw_pixel(x - dx, y + dy, color);
            gfx_draw_pixel(x + dx, y - dy, color);
            gfx_draw_pixel(x - dx, y - dy, color);
            gfx_draw_pixel(x + dy, y + dx, color);
            gfx_draw_pixel(x - dy, y + dx, color);
            gfx_draw_pixel(x + dy, y - dx, color);
            gfx_draw_pixel(x - dy, y - dx, color);

            if (d < 0) {
                d += 2 * dx + 3;
            } else {
                d += 2 * (dx - dy) + 5;
                dy--;
            }
            dx++;
        }
    }
}

void gfx_draw_button(gfx_button_t* btn) {
    // Check if the button is pressed
    bool is_pressed = (btn->flags & GFX_BTNSTATE_PRESSED) != 0;
    uint8_t charlen = strlen(btn->btn_caption);
    // Draw the button rectangle
    if (is_pressed) {
        // Draw a filled rectangle if the button is pressed
        gfx_draw_rect(btn->x, btn->y, charlen*8+4, 12, btn->color_index_bg, GFX_FLAG_FILLED); // 1 indicates filled
    } else {
        // Draw only the outline of the rectangle if the button is not pressed
        gfx_draw_rect(btn->x, btn->y, charlen*8+4, 12, btn->color_index_bg, 0); // 0 indicates outline
    }

    // Draw the button caption
    gfx_draw_text(btn->x+2, btn->y+2, (char*)btn->btn_caption, btn->color_index_txt, COL_TRANSPARENT);
}



void gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx, uint8_t bg_index)
{
    if (!active_pixelbuffer){
        return;
    }
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
				*buffer_tip = bg_index;
			}
			mask = mask << 1;
			buffer_tip++;
		}
	}
}

void gfx_draw_text(uint16_t x, uint16_t y, char* txt, uint8_t color_idx, uint8_t bg_index)
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

					if (txt[i+2]=='R'){
						color_idx = initial_color;
						i+=2;
						break;
					} else {
						strncpy(number,txt+i+2,3);
						color_idx = atoi(number);
						i+=4;
						break;
					}
				}
			default:
				gfx_draw_char(x,y,ch,color_idx,bg_index);
				x+=8;
				break;
		}
	}
}
// void gfx_draw_printf(uint16_t x,uint16_t y,uint8_t color_idx, uint8_t bg_index,const char *format, ...) {
// 	va_list arg;
//     va_start(arg, format);
//     char temp[64];
//     size_t len = ng_snprintf(buf,40,format, arg);
//     //size_t len = vsnprintf(temp, sizeof(temp), format, arg);
//     va_end(arg);
// 	gfx_draw_text(x,y,temp, color_idx, bg_index);
// }

gfx_tilesheet_current_info_t current_tilesheet_info;


void _gfx_tilesheet_prepare_info(void){
    assert(current_tilesheet_info.ts!=NULL);
    gfx_tilesheet_current_info_t* cur = &current_tilesheet_info;
    cur->tile_height = cur->px_height * cur->ts->data.tile_height;
    cur->tile_width  = cur->px_width * cur->ts->data.tile_width;
}

void gfx_tilesheet_current_set_pixel_size(uint8_t px_width,uint8_t px_height){
    current_tilesheet_info.px_width = px_width;
    current_tilesheet_info.px_height = px_height;
    if (current_tilesheet_info.ts!=NULL){
        _gfx_tilesheet_prepare_info();
    }
}

void gfx_tilesheet_current_set(gfx_tilesheet_data_t* tsdata){
    gfx_tilesheet_t* ts = id_get_ptr(tsdata->ts_id);
    current_tilesheet_info.ts = ts;
    _gfx_tilesheet_prepare_info();
}

void gfx_draw_tile(int16_t _x, int16_t _y, uint8_t tile_number) {
    assert(current_tilesheet_info.ts!=NULL && "NO TILESHEET SET!");

    gfx_tilesheet_t* ts = current_tilesheet_info.ts;
    gfx_tilesheet_data_t* tsdata = &ts->data;

    // TODO: Is this needed when I draw to the pixelbuffer? I should only take assets into the RAM if renderer on the beam
    uint8_t *tile_ptr = gfx_tilesheet_get_chached_tile(ts,tile_number);

    uint8_t format = flags_mask_value(tsdata->type, ASSET_TYPE_FILEFORMAT_MASK);
    uint8_t offset_left = 0;
    uint8_t offset_top = 0;
    uint8_t offset_right = 0;
    uint8_t offset_bottom = 0;
    uint8_t offset_width = ts->data.tile_width;
    uint8_t offset_height = ts->data.tile_height;
    if (format == ASSET_TYPE_FILEFORMAT_2)
    {
        uint8_t oX = *(tile_ptr++);
        uint8_t oY = *tile_ptr++;
        offset_width = *tile_ptr++;
        offset_height = *tile_ptr++;

        offset_left = oX;
        offset_top = oY;
        offset_right = ts->data.tile_width - oX - offset_width;
        offset_bottom = ts->data.tile_height - oY - offset_height;
    }

    if (_x+offset_width<0 || _x>active_pixelbuffer->width){
        return;
    }

    uint16_t start_x = _x + offset_left;
    uint16_t start_y = _y + offset_top;

    for (int16_t y = start_y, yEnd = offset_height+start_y; y < yEnd; y++){
        for (int16_t x = start_x, xEnd=start_x+offset_width; x < xEnd; x++){
            
            uint8_t current_idx = *tile_ptr++;

            // TODO: make a version that checks once if the sprite is contained completely by the pixelbuffer so we won't have to check each pixel
            if ((x < 0 || x >= active_pixelbuffer->width) || (y < 0 || y >= active_pixelbuffer->height)
            ){
                continue;
            }

            if (current_idx!=255){
                gfx_draw_pixel(x,y,current_idx);
            } else {
                int a=0;
            }
        }
    }
}

void gfx_draw_bitmap(int16_t x, int16_t y, uint8_t asset_id,uint8_t px_size_4_4){

}

//// void gfx_draw_tilemap(int16_t x,int16_t y, gfx_tilemap_data_t* tilemap) {
void gfx_draw_tilemap_layer(int16_t x,int16_t y, gfx_tilemap_layer_t* tilemap_layer){
    gfx_internal_tilemap_t* tm_internal = id_get_ptr(tilemap_layer->tm_handle);
    gfx_tilemap_t* tm = tm_internal->tilemap_data;

    gfx_tilesheet_t* ts = id_get_ptr(tm->tilesheet_id);
    
    current_tilesheet_info.ts = ts;
    _gfx_tilesheet_prepare_info();
    gfx_tilesheet_data_t* tsdata = &ts->data;

    for (int tY=0,tEnd=tm->tileamount_vertical;tY < tEnd;tY++){
        for (int tX=0,tEnd=tm->tileamount_horizontal;tX < tEnd;tX++){
            uint8_t tile_id = tilemap_layer->data[tX+tY*tm->tileamount_horizontal];
            if (tile_id == 255){
                continue;
            }
            gfx_draw_tile(
                x+tX*tsdata->tile_width,
                y+tY*tsdata->tile_height,
                tile_id
            );
        }
    }
}

void gfx_draw_printf(uint16_t x, uint16_t y, uint8_t color_idx, uint8_t bg_idx, const char *format, ...) {
    // Temporary buffer to hold the formatted string
    char temp_txt_buf[80]={0};

    // Initialize the variable argument list
    va_list args;
    va_start(args, format);

    // Format the string using snprintf and the provided arguments
    vsnprintf(temp_txt_buf, sizeof(temp_txt_buf), format, args);

    // End the variable argument list
    va_end(args);

    // Draw the formatted text on the screen
    gfx_draw_text(x, y, temp_txt_buf, color_idx, bg_idx);
}

// █▀▄ ██▀ ██▄ █ █ ▄▀  █▀ █ █ █▄ █ ▄▀▀ ▀█▀ █ ▄▀▄ █▄ █ ▄▀▀ 
// █▄▀ █▄▄ █▄█ ▀▄█ ▀▄█ █▀ ▀▄█ █ ▀█ ▀▄▄  █  █ ▀▄▀ █ ▀█ ▄█▀ 

void gfx_debug_drawinfo_pixelbuffer(uint16_t x, uint16_t y, gfx_pixelbuffer_t* pxb, uint8_t color_idx, uint8_t bg_idx)
{
    uint8_t px,py;
    flags_unpack_4_4(pxb->pixel_size,px,py);
    snprintf(temp_txt_buf,40,"w:%d h:%d p:%d|%d",pxb->width,pxb->height,pxb->x,pxb->y);
    gfx_draw_text(x,y,temp_txt_buf,color_idx,bg_idx);
    snprintf(temp_txt_buf,40,"f:%s s:%d|%d",utils_char_to_binstring(pxb->flags),px,py);
    gfx_draw_text(x,y+8,temp_txt_buf,color_idx,bg_idx);
}

void gfx_debug_drawinfo_keyboard(uint16_t x, uint16_t y, keyboard_mapping_t* keyb,uint8_t coltext, uint8_t col_bg) {
    snprintf(temp_txt_buf,40,"pressed: %s",utils_char_to_binstring(keyb->key_pressed));    
    gfx_draw_text(x,y,temp_txt_buf,coltext,col_bg);
    snprintf(temp_txt_buf,40,"down: %s",utils_char_to_binstring(keyb->key_down));    
    gfx_draw_text(x,y+8,temp_txt_buf,coltext,col_bg);
    snprintf(temp_txt_buf,40,"released: %s",utils_char_to_binstring(keyb->key_released));    
    gfx_draw_text(x,y+16,temp_txt_buf,coltext,col_bg);
}

void gfx_debug_drawinfo_mouse(uint16_t x, uint16_t y,uint8_t coltext, uint8_t col_bg) {
    gfx_draw_printf(x,y,coltext,col_bg,"m: x:%d y:%d w:%d\np:%d d:%d r:%d"
                        ,*mm_mouse_x
                        ,*mm_mouse_y
                        ,*mm_mouse_wheel
                        ,*mm_mouse_btn_state_pressed
                        ,*mm_mouse_btn_state
                        ,*mm_mouse_btn_state_released
    );
}

void gfx_debug_drawinfo_gamepad(uint16_t x, uint16_t y,uint8_t gamepad_idx, uint8_t coltext, uint8_t col_bg) {
    gamepad_state_t* gp_pressed = &mm_gamepad_pressed[gamepad_idx];
    gamepad_state_t* gp_down = &mm_gamepad_down[gamepad_idx];
    gamepad_state_t* gp_released = &mm_gamepad_released[gamepad_idx];

    gfx_draw_printf(x,y,coltext,col_bg,"g%d p:%d|%d d:%d|%d r:%d|%d"
            ,gamepad_idx
            ,gp_pressed->controls
            ,gp_pressed->buttons
            ,gp_down->controls
            ,gp_down->buttons
            ,gp_released->controls
            ,gp_released->buttons
    );
}
