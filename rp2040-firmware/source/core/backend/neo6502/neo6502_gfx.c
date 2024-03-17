#include "../../../ng_gfx.h"
#include "neo6502.h"
#include "../../../api/ng_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "pico/sem.h"
#include "pico/lock_core.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "tmds_encode.h"
#include "common_dvi_pin_configs.h"
#include "sprite.h"
#include "tile.h"

// Pick one:
#define MODE_640x480_60Hz
// #define MODE_800x480_60Hz
// #define MODE_800x600_60Hz
// #define MODE_960x540p_60Hz
// #define MODE_1280x720_30Hz

#if defined(MODE_640x480_60Hz)
// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

#elif defined(MODE_800x480_60Hz) 
#define FRAME_WIDTH 400
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_800x480p_60hz

#elif defined(MODE_800x600_60Hz)
// DVDD 1.3V, going downhill with a tailwind
#define FRAME_WIDTH 400
#define FRAME_HEIGHT 300
#define VREG_VSEL VREG_VOLTAGE_1_30
#define DVI_TIMING dvi_timing_800x600p_60hz

#elif defined(MODE_960x540p_60Hz)
// DVDD 1.25V (slower silicon may need the full 1.3, or just not work)
// Frame resolution is almost the same as a PSP :)
#define FRAME_WIDTH 480
#define FRAME_HEIGHT 270
#define VREG_VSEL VREG_VOLTAGE_1_25
#define DVI_TIMING dvi_timing_960x540p_60hz

#elif defined(MODE_1280x720_30Hz)
// 1280x720p 30 Hz (nonstandard)
// DVDD 1.25V (slower silicon may need the full 1.3, or just not work)
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 360
#define VREG_VSEL VREG_VOLTAGE_1_25
#define DVI_TIMING dvi_timing_1280x720p_30hz

#else
#error "Select a video mode!"
#endif

extern uint8_t default_allocation_segment;
extern bool requested_renderqueue_apply;
extern ng_mem_block_t* renderqueue_1[GFX_RENDERQUEUE_MAX_ELEMENTS];
extern ng_mem_block_t* renderqueue_2[GFX_RENDERQUEUE_MAX_ELEMENTS];

extern uint8_t renderqueue_request_amount;
extern ng_mem_block_t** renderqueue_current;
extern ng_mem_block_t** renderqueue_request;
extern uint32_t frame;

// ----------------------------------------------------------------------------
// Rendering/animation stuff
struct dvi_inst dvi0;


// Pico HDMI for Olimex Neo6502 
static const struct dvi_serialiser_cfg _pico_neo6502_cfg = {
.pio = DVI_DEFAULT_PIO_INST,
.sm_tmds = {0, 1, 2},
.pins_tmds = {14, 18, 16},
.pins_clk = 12,
.invert_diffpairs = true
};

//uint16_t __scratch_x("render") __attribute__((aligned(4))) core1_scanbuf[FRAME_WIDTH*2];
// uint16_t  __attribute__((aligned(4))) core1_scanbuf[FRAME_WIDTH*4];
uint16_t /*__scratch_x("render")*/ __attribute__((aligned(4))) core1_scanbuf[FRAME_WIDTH+20];

void neo6502_copy_from_flash_to_ram(ng_mem_block_t* block, uint8_t segment_id,uint8_t usage_type,void* data,uint32_t size){
	bool success = ng_mem_allocate_block(segment_id,size,usage_type, block);
	assert(success && "could not allocate any more RAM");
	memcpy(block->data,data,size);
}

void encode_scanline(uint16_t *pixbuf, uint32_t *tmdsbuf) {
	uint pixwidth = dvi0.timing->h_active_pixels;
	uint16_t pw2 = pixwidth >> 1;
    uint words_per_channel = pw2;
	tmds_encode_data_channel_16bpp((uint32_t*)pixbuf, tmdsbuf + 0 * words_per_channel, pw2, DVI_16BPP_BLUE_MSB,  DVI_16BPP_BLUE_LSB );
	tmds_encode_data_channel_16bpp((uint32_t*)pixbuf, tmdsbuf + 1 * words_per_channel, pw2, DVI_16BPP_GREEN_MSB, DVI_16BPP_GREEN_LSB);
	tmds_encode_data_channel_16bpp((uint32_t*)pixbuf, tmdsbuf + 2 * words_per_channel, pw2, DVI_16BPP_RED_MSB,   DVI_16BPP_RED_LSB  );
}

int a = 0;

#define CACHE_SIZE 16
uint16_t __attribute__((aligned(4))) scanbuf_cache[FRAME_WIDTH*CACHE_SIZE];
queue_t cache_free;
queue_t cache_filled;

/// @brief render loop on core1
void __not_in_flash_func(core1_main()) {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	// while (queue_is_empty(&dvi0.q_tmds_valid))
	// 	__wfe();
    queue_init(&cache_free,sizeof(uint16_t*),CACHE_SIZE);
    queue_init(&cache_filled,sizeof(uint16_t*),CACHE_SIZE);
    uint16_t* current_tip = scanbuf_cache;
    for (int i=0;i<CACHE_SIZE;i++){
        queue_add_blocking_u32(&cache_free,current_tip);
        current_tip+=FRAME_WIDTH;
    }
	dvi_start(&dvi0);
	//dvi_scanbuf_main_16bpp(&dvi0);
    uint32_t *tmds0;
    uint16_t *pixbuf;
   	while (1) {
		for (uint y = 0; y < FRAME_HEIGHT; y ++) {
            // bool blocked = queue_try_remove_u32(&dvi0.q_tmds_free,&tmds0);
            // if (blocked){
            //     queue_remove_blocking_u32(&cache_free,&pixbuf);
 	        //     gfx_render_scanline(pixbuf, y);
            //     queue_add_blocking_u32(&cache_filled,pixbuf);
            //     continue;
            // }
            // //queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmds0);
 	
            // if (requested_renderqueue_apply){
            //     requested_renderqueue_apply = false;
            //     ng_mem_block_t** save_current = renderqueue_current;
            //     renderqueue_current = renderqueue_request;
            //     renderqueue_request = save_current;
            // }

            // bool found = queue_try_remove_u32(&cache_filled,&pixbuf);
            // if (found){
    		// 	encode_scanline(pixbuf, tmds0);
            // } else {
     	    //     gfx_render_scanline(core1_scanbuf, y);
            //     encode_scanline(core1_scanbuf, tmds0);
            // }
            // queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmds0);

            queue_remove_blocking_u32(&dvi0.q_tmds_free,&tmds0);
 	
            if (requested_renderqueue_apply){
                requested_renderqueue_apply = false;
                ng_mem_block_t** save_current = renderqueue_current;
                renderqueue_current = renderqueue_request;
                renderqueue_request = save_current;
            }

            gfx_render_scanline(core1_scanbuf, y);
            encode_scanline(core1_scanbuf, tmds0);
            queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmds0);

		}
	} 

	__builtin_unreachable();
}

/// @brief callback to render current scanline
/// @param  
/// @return 
void __not_in_flash_func(core1_scanline_callback)() {
	// Discard any scanline pointers passed back
	uint16_t *bufptr;
	queue_try_remove_u32(&dvi0.q_colour_free, &bufptr);
	// // Note first two scanlines are pushed before DVI start
	static uint scanline = 2;
	//bufptr = &framebuf[FRAME_WIDTH * scanline];
 	
	bufptr = &core1_scanbuf[(scanline & 1)*FRAME_WIDTH]; 
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

	if (requested_renderqueue_apply){
		requested_renderqueue_apply = false;
		ng_mem_block_t** save_current = renderqueue_current;
		renderqueue_current = renderqueue_request;
		renderqueue_request = save_current;
	}

	scanline++;
	if (scanline >= FRAME_HEIGHT){
		frame++;
		scanline = 0;
	}
	
	bufptr = &core1_scanbuf[(scanline & 1)*FRAME_WIDTH]; // alternate between odd or even intermediate lines encoded in 565-format
 	gfx_render_scanline(bufptr, scanline);
}

void gfx_backend_init()
{
 	vreg_set_voltage(VREG_VSEL);
	sleep_ms(10);
	// Run system at TMDS bit clock
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

	setup_default_uart();

	// gpio_init(LED_PIN);
	// gpio_set_dir(LED_PIN, GPIO_OUT);

	printf("Configuring DVI\n");

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = _pico_neo6502_cfg;
	//dvi0.scanline_callback = core1_scanline_callback;
	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

	// Once we've given core 1 the framebuffer, it will just keep on displaying
	// it without any intervention from core 0
	//sprite_fill16(framebuf, 0xffff, FRAME_WIDTH * FRAME_HEIGHT);
	
	// uint16_t *bufptr = &core1_scanbuf[0];
	// gfx_render_scanline(bufptr,0);
	
	// queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
	// bufptr += FRAME_WIDTH;
	// queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
	// bufptr += FRAME_WIDTH;
	// queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
	// bufptr += FRAME_WIDTH;
	// queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

	printf("Core 1 start\n");
	multicore_launch_core1(core1_main);

	printf("Start rendering\n");
   
}

void gfx_backend_update()
{

}

void* gfx_tilesheet_get_chached_tile(gfx_tilesheet_t* ts,uint8_t tile_id){
	uint8_t* data = ts->cached_tile_ptrs[tile_id];
	if (data!=NULL){
		return data; 
	}
	
    uint8_t format = flags_mask_value(ts->data.type,ASSET_TYPE_FILEFORMAT_MASK);
    
    uint16_t size_per_tile = 0;
    uint8_t* tile_ptr = NULL;
    if (format == ASSET_TYPE_FILEFORMAT_1){  
        size_per_tile = (ts->data.tile_width*ts->data.tile_height);
        tile_ptr = ts->tilesheet_data_raw + tile_id * size_per_tile;
    } 
    else if (format == ASSET_TYPE_FILEFORMAT_2) {
        uint16_t* tile_offsets = (uint16_t*)ts->tilesheet_data_raw;
        uint16_t tile_offset = tile_offsets[tile_id];
        tile_ptr = ts->tilesheet_data_raw + tile_offset;
        uint8_t* peek_ptr = tile_ptr;
        uint8_t oX = *peek_ptr++;
        uint8_t oY = *peek_ptr++;
        uint8_t oW = *peek_ptr++;
        uint8_t oH = *peek_ptr++;
        size_per_tile = oW*oH;
    }
    void* dest = ng_mem_allocate(default_allocation_segment, size_per_tile + 4);
    memcpy(dest, tile_ptr, size_per_tile + 4);
    ts->cached_tile_ptrs[tile_id]=dest;
    return dest;
}