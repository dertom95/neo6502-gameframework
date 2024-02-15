#ifdef __KINC__

#include "kinc_ng.h"
#include <stdint.h>
#include <string.h>

#include "../../../ng_gfx.h"
#include "../../../api/ng_config.h"
#include "../../../ng_mem.h"

#include <kinc/graphics1/graphics.h>
#include <kinc/system.h>
#include <kinc/log.h>
#include <kinc/window.h>
#include <kinc/threads/thread.h>

extern bool requested_renderqueue_apply;
extern ng_mem_block_t* renderqueue_1[GFX_RENDERQUEUE_MAX_ELEMENTS];
extern ng_mem_block_t* renderqueue_2[GFX_RENDERQUEUE_MAX_ELEMENTS];

extern uint8_t renderqueue_request_amount;
extern ng_mem_block_t** renderqueue_current;
extern ng_mem_block_t** renderqueue_request;

uint16_t* pixbuf;
uint32_t framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
#define FRAMEBUFFER_SIZE (SCREEN_HEIGHT*SCREEN_WIDTH*4)

uint16_t window_width=0, window_height=0;
float    window_factor_x=1.0f, window_factor_y=1.0f;


uint32_t convertColor565toRGBA(uint16_t color565) {
    // Extract the color components from the 16-bit color value
    uint32_t red = (color565 >> 11) & 0x1F;
    uint32_t green = (color565 >> 5) & 0x3F;
    uint32_t blue = color565 & 0x1F;

    // Scale the color components to 8 bits
    uint32_t red8 = (red * 255) / 31;
    uint32_t green8 = (green * 255) / 63;
    uint32_t blue8 = (blue * 255) / 31;

    // Create the 32-bit RGBA color value
    uint32_t rgba = 0xFF << 24 | (red8 ) | (green8 << 8) | (blue8 << 16);
    return rgba;
}

void flush_buffer() {
    memcpy(kinc_internal_g1_image, framebuffer, FRAMEBUFFER_SIZE);
}

static void callback_window_resized(int x, int y,void* data){
    window_width=x;
    window_height=y;
    window_factor_x = SCREEN_WIDTH / (float)window_width;
    window_factor_y = SCREEN_HEIGHT / (float)window_height;
}

void gfx_backend_init()
{
    pixbuf = ng_mem_allocate(SEGMENT_GFX_DATA,SCREEN_WIDTH*sizeof(uint16_t*));
    
    kinc_framebuffer_options_t framebuffer_opts;
    kinc_framebuffer_options_set_defaults(&framebuffer_opts);
    framebuffer_opts.vertical_sync=false;
    framebuffer_opts.frequency=10000;
    int win = kinc_init("n6502", SCREEN_WIDTH, SCREEN_HEIGHT, NULL, &framebuffer_opts);
    kinc_log(KINC_LOG_LEVEL_INFO, "vga_setup");
    kinc_g1_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    kinc_window_set_resize_callback(win,&callback_window_resized,NULL);
}

void gfx_backend_update()
{
    if (requested_renderqueue_apply){
		requested_renderqueue_apply = false;
		ng_mem_block_t** save_current = renderqueue_current;
		renderqueue_current = renderqueue_request;
		renderqueue_request = save_current;
	}
    kinc_g1_begin();
    uint32_t* screenTip = framebuffer;
    // for (int i=0;i<600;i++){
       //  kinc_g1_set_pixel(i%20,i%20,1,0,0);
    // }
    for (int y=0;y<SCREEN_HEIGHT;y++){
        gfx_render_scanline(pixbuf,y);
        uint16_t* _scanline = pixbuf;
        uint16_t count = SCREEN_WIDTH;
        while (count--){
            *(screenTip++)=convertColor565toRGBA(*(_scanline++));
        }
    }
    flush_buffer();
    kinc_g1_end();
}

void* gfx_tilesheet_get_chached_tile(gfx_tilesheet_t* ts,uint8_t tile_id){
	uint8_t* data = ts->cached_tile_ptrs[tile_id];
	if (data!=NULL){
		return data; 
	}
	
	uint16_t size_per_tile = (ts->tile_width*ts->tile_height);
	uint8_t* tile_ptr = ts->tilesheet_data_raw + tile_id * size_per_tile;
	
	ts->cached_tile_ptrs[tile_id]=tile_ptr;
    return tile_ptr;
}



#endif