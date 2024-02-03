#include "ng_gfx.h"

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "pico/sem.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "tmds_encode.h"
#include "common_dvi_pin_configs.h"
#include "sprite.h"
#include "tile.h"

#include "tilemap.h"
#include "gen/font_8.h"
#include "gen/color_palette.h"
//#include "zelda_mini_plus_walk_rgab5515.h"
#include <string.h>

#include "ng_io.h"
#include "api/ng_api.h"
#include <math.h>

#include "core/wdc65C02cpu.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

uint8_t* pixelbuffer=NULL; // indexed 8bit
uint8_t* font=NULL; 		  // 1bpp

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



// ----------------------------------------------------------------------------
// Rendering/animation stuff


// Pico HDMI for Olimex Neo6502 
static const struct dvi_serialiser_cfg _pico_neo6502_cfg = {
.pio = DVI_DEFAULT_PIO_INST,
.sm_tmds = {0, 1, 2},
.pins_tmds = {14, 18, 16},
.pins_clk = 12,
.invert_diffpairs = true
};

uint16_t gfx_color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}


static inline int clip(int x, int min, int max) {
	return x < min ? min : x > max ? max : x;
}

uint8_t sprite1_data[] = {
	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,	

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,	

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,

	8,10,10,10,8,
	10,8,8,8,10,
	10,8,10,8,10,
	10,8,8,8,10,
	8,10,10,10,8,							
};

uint16_t* sp16 = NULL;

#define SPRITE_CONVERTED (1 << 0)

gfx_sprite_t test_sprite = {
	.width=20,
	.height=20,
	.flags=0,
	.tiles=1,
	.data=sprite1_data
};

void gfx_load_sprite8bpp(gfx_sprite_t* sprite){
	if ((sprite->flags & SPRITE_CONVERTED)==0){
		int count = sprite->width*sprite->height;
		uint16_t* data = malloc(count * sizeof(uint16_t));
		uint16_t* buffer = data;
		uint8_t* source = sprite->data;
		while(count--){
			*(data++)=color_palette[*(source++)];
		}
		sprite->data = buffer;
		sprite->flags |= SPRITE_CONVERTED;
	}
}

void game_init(game_state_t *state) {
	state->cam_x = 0;
	state->cam_y = 0;
	state->frame_ctr = 0;


	gfx_load_sprite8bpp(&test_sprite);

	for (int i = 0; i < N_CHARACTERS; ++i) {
		state->chars[i].dir = (rand() >> 16) & 0x3;
		state->chars[i].anim_frame = 0;
		state->chars[i].xmin = 8;
		state->chars[i].ymin = -6;
		state->chars[i].xmax = MAP_WIDTH - 24;
		state->chars[i].ymax = 128+87;
		state->chars[i].pos_x = rand() % 320;
		state->chars[i].pos_y = rand() % 220;
		state->chars[i].tile = 102;
		state->chars[i].tilestride = 17;
		state->chars[i].ntiles = 2;
	}
}

void __not_in_flash_func(gfx_tile_set_color)(uint8_t tX,uint8_t tY,uint8_t col)
{
    for (int y=tY*8,yEnd=(tY*8+8);y<yEnd;y++){
        for (int x=tX*8,xEnd=(tX*8+8);x<xEnd;x++){
            gfx_draw_pixel(x,y,col);
        }
    }
}

void update(game_state_t *state) {
	static bool cointoss = false;
	if ((cointoss = !cointoss))
		return;

	state->frame_ctr++;

	const int CAMERA_SPEED = 3;
	if (state->frame_ctr % 200 < 50)
		state->cam_x += CAMERA_SPEED;
	else if (state->frame_ctr % 200 < 100)
		state->cam_y += CAMERA_SPEED;
	else if (state->frame_ctr % 200 < 150)
		state->cam_x -= CAMERA_SPEED;
	else
		state->cam_y -= CAMERA_SPEED;
	state->cam_x = clip(state->cam_x, 0, MAP_WIDTH - FRAME_WIDTH);
	state->cam_y = clip(state->cam_y, 0, MAP_HEIGHT - FRAME_HEIGHT);

	const int CHAR_SPEED = 2;
	for (int i = 0; i < N_CHARACTERS; ++i) {
		character_t *ch = &state->chars[i];
		ch->pos_x++;

		if (ch->pos_x>310){
			ch->pos_x=0;
		}		
	}

	static uint heartbeat = 0;
	if (++heartbeat >= 30) {
		heartbeat = 0;
	}
}


static void __not_in_flash_func(render_scanline)(uint16_t *pixbuf, uint y, const game_state_t *gstate) {
	// tilebg_t bg = {
	// 	.xscroll = gstate->cam_x,
	// 	.yscroll = gstate->cam_y,
	// 	.tileset = zelda_mini_plus_walk,
	// 	.tilemap = tilemap,
	// 	.log_size_x = 9,
	// 	.log_size_y = 8,
	// 	.tilesize = TILESIZE_16,
	// 	.fill_loop = (tile_loop_t)tile16_16px_loop
	// };

	sprite_t sp = {
		.log_size = 4,
		.has_opacity_metadata = false,
	};

	//tile16(pixbuf, &bg, y, FRAME_WIDTH);

	uint16_t* write_buf = pixbuf;
	{
		uint8_t* buffer = &pixelbuffer[y*320];
		for (int i=0;i<320;i++){
			// if ((y*320+i)>(FRAME_WIDTH*FRAME_HEIGHT)){
			// 	continue;
			// }
			uint8_t data = *(buffer++);
			// if (data==0){
			// 	write_buf++;
			// 	continue;
			// }
			*(write_buf++)=color_palette[data];
			//pixbuf[i]=gfx_color565(254,254,254);
		}
	}

	for (int i = 0; i < N_CHARACTERS; ++i) {
		const character_t *ch = &gstate->chars[i];
		gfx_sprite_t* sprite = &test_sprite;

		if (ch->pos_y > y || (ch->pos_y+sprite->height)<y){
			continue;
		}
		uint8_t count = min(sprite->width,FRAME_WIDTH-ch->pos_x);
		#if 0
			//8bpp
			uint8_t* data = sprite->data + (y - ch->pos_y)*sprite->width;
			write_buf = pixbuf+ch->pos_x;
			while (count--){
				*(write_buf++)=color_palette[*(data++)];
			}
		#else
			// data converted to 16bpp
			uint16_t* data = sprite->data;
			data +=(y - ch->pos_y)*sprite->width;
			write_buf = pixbuf+ch->pos_x;
			while (count--){
				*(write_buf++)=*(data++);
			}
		#endif
	}



	
	//if (y<50)

}

// ----------------------------------------------------------------------------
// DVI setup & launch

struct dvi_inst dvi0;
game_state_t state;
//uint16_t framebuf[FRAME_WIDTH * FRAME_HEIGHT];

uint16_t __scratch_x("render") __attribute__((aligned(4))) core1_scanbuf[FRAME_WIDTH*2];

// - Core 0 pops two TMDS buffers
// - Passes one to core 1
// - Renders own buffer and pushes to DVI queue  <- core 1 waits here before starting DVI
// - Retrieves core 1's TMDS buffer and pushes that to DVI queue as well

// void encode_scanline(uint16_t *pixbuf, uint32_t *tmdsbuf) {
// 	uint pixwidth = dvi0.timing->h_active_pixels;
// 	uint words_per_channel = pixwidth / DVI_SYMBOLS_PER_WORD;
// 	tmds_encode_data_channel_16bpp((uint32_t*)pixbuf, tmdsbuf + 0 * words_per_channel, pixwidth / 2, DVI_16BPP_BLUE_MSB,  DVI_16BPP_BLUE_LSB );
// 	tmds_encode_data_channel_16bpp((uint32_t*)pixbuf, tmdsbuf + 1 * words_per_channel, pixwidth / 2, DVI_16BPP_GREEN_MSB, DVI_16BPP_GREEN_LSB);
// 	tmds_encode_data_channel_16bpp((uint32_t*)pixbuf, tmdsbuf + 2 * words_per_channel, pixwidth / 2, DVI_16BPP_RED_MSB,   DVI_16BPP_RED_LSB  );
// }

// void core1_scanline_callback() {
// 	// Discard any scanline pointers passed back
// 	uint16_t *bufptr;
// 	while (queue_try_remove_u32(&dvi0.q_colour_free, &bufptr))
// 		;
// 	// // Note first two scanlines are pushed before DVI start
// 	static uint scanline = 2;
// 	bufptr = &core1_scanbuf[(scanline & 1)*FRAME_WIDTH]; // alternate between odd or even intermediate lines encoded in 565-format
// 	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

// 	bufptr = &core1_scanbuf[(scanline & 1)*FRAME_WIDTH]; // alternate between odd or even intermediate lines encoded in 565-format
// 	render_scanline(bufptr, scanline, &state);
// 	scanline = (scanline + 1) % FRAME_HEIGHT;
// }

// uint16_t iCount;
// uint16_t addr;
// uint8_t data;



void core1_main() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	dvi_start(&dvi0);
	dvi_scanbuf_main_16bpp(&dvi0);
	__builtin_unreachable();
}

uint frame;

void __not_in_flash_func(core1_scanline_callback)() {
	// Discard any scanline pointers passed back
	uint16_t *bufptr;
	while (queue_try_remove_u32(&dvi0.q_colour_free, &bufptr))
		;
	// // Note first two scanlines are pushed before DVI start
	static uint scanline = 2;
	//bufptr = &framebuf[FRAME_WIDTH * scanline];
 	
	bufptr = &core1_scanbuf[(scanline & 1)*FRAME_WIDTH]; 
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

	scanline++;
	if (scanline >= FRAME_HEIGHT){
		frame++;
		scanline = 0;
	}
	
	bufptr = &core1_scanbuf[(scanline & 1)*FRAME_WIDTH]; // alternate between odd or even intermediate lines encoded in 565-format
 	render_scanline(bufptr, scanline, &state);
}




// void core1_main() {
// 	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
// 	dvi_start(&dvi0);
// 	dvi_scanbuf_main_16bpp(&dvi0);
// 	__builtin_unreachable();

// 	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
// 	while (queue_is_empty(&dvi0.q_tmds_valid))
// 		__wfe();
// 	dvi_start(&dvi0);
//     wdc65C02cpu_init();                                                         // Set up the 65C02
//     wdc65C02cpu_reset();
// 	while (1) {
// 		for (uint y = 1; y < FRAME_HEIGHT; y += 2) {
// 			tick6502();
// 			tick6502();
// 			render_scanline(core1_scanbuf, y, &state);
// 			uint32_t *tmdsbuf;
// 			while(1){
// 				tick6502();
// 				tick6502();
// 			  	if (multicore_fifo_pop_timeout_us(0,&tmdsbuf)){
// 					encode_scanline(core1_scanbuf, tmdsbuf);

// 					while (1){
// 						tick6502();
// 						tick6502();
// 						if (multicore_fifo_push_timeout_us((uintptr_t)tmdsbuf,0)){
// 							break;
// 						}
// 						tick6502();
// 						tick6502();
// 					}
// 					break;
// 				}
// 				tick6502();
// 			}
				

// //			uint32_t *tmdsbuf = (uint32_t*)multicore_fifo_pop_blocking();
// 		}
// 	}
// }

// void gfx_init() {
// 	// canvas pixelbuffer
// 	pixelbuffer = malloc(FRAME_WIDTH * FRAME_HEIGHT);
// 	memset(pixelbuffer,0,FRAME_WIDTH * FRAME_HEIGHT);

// 	font = (uint8_t*)bin2c_font8_bin;

// 	vreg_set_voltage(VREG_VSEL);
// 	sleep_ms(10);
// 	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

// 	dvi0.timing = &DVI_TIMING;
// 	dvi0.ser_cfg = _pico_neo6502_cfg;
// 	dvi0.scanline_callback = core1_scanline_callback;
// 	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());
// 	sleep_ms(10);

// 	uint16_t *bufptr = &core1_scanbuf[0];
// 	render_scanline(bufptr,0,&state);
// 	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
// 	bufptr = &core1_scanbuf[FRAME_HEIGHT];
// 	render_scanline(bufptr,1,&state);
// 	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

// 	//printf("Core 1 start\n");
// 	multicore_launch_core1(core1_main);

// 	//printf("Start rendering\n");
// 	game_init(&state);
// }


void gfx_init()
{
	game_init(&state);    

	
	pixelbuffer = malloc(FRAME_WIDTH * FRAME_HEIGHT);
 	memset(pixelbuffer,0,FRAME_WIDTH * FRAME_HEIGHT);

 	font = (uint8_t*)bin2c_font8_bin;

	
	vreg_set_voltage(VREG_VSEL);
	sleep_ms(10);
#ifdef RUN_FROM_CRYSTAL
	set_sys_clock_khz(12000, true);
#else
	// Run system at TMDS bit clock
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);
#endif

	setup_default_uart();

	// gpio_init(LED_PIN);
	// gpio_set_dir(LED_PIN, GPIO_OUT);

	printf("Configuring DVI\n");

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = _pico_neo6502_cfg;
	dvi0.scanline_callback = core1_scanline_callback;
	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

	// Once we've given core 1 the framebuffer, it will just keep on displaying
	// it without any intervention from core 0
	//sprite_fill16(framebuf, 0xffff, FRAME_WIDTH * FRAME_HEIGHT);
	
	uint16_t *bufptr = &core1_scanbuf[0];
	render_scanline(bufptr,0,&state);
	
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
	bufptr += FRAME_WIDTH;
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

	printf("Core 1 start\n");
	multicore_launch_core1(core1_main);

	printf("Start rendering\n");


}

void gfx_draw()
{
	// for (uint y = 0; y < FRAME_HEIGHT; y += 2) {
	// 	uint32_t *tmds0, *tmds1;
	// 	queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmds0);
	// 	queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmds1);
	// 	multicore_fifo_push_blocking((uintptr_t)tmds1);
	// 	render_scanline(core0_scanbuf, y, &state);
	// 	encode_scanline(core0_scanbuf, tmds0);
	// 	queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmds0);
	// 	tmds1 = (uint32_t*)multicore_fifo_pop_blocking();
	// 	queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmds1);
	// }
	for (int y = 0; y < FRAME_HEIGHT; ++y) {
		for (int x = 0; x < FRAME_WIDTH; ++x) {
			const float scale = FRAME_HEIGHT / 2;
			float cr = ((float)x - FRAME_WIDTH / 2) / scale - 0.5f;
			float ci = ((float)y - FRAME_HEIGHT / 2) / scale;
			float zr = cr;
			float zi = ci;
			int iters;
			const int max_iters = 255;
			for (iters = 0; iters < max_iters; ++iters) {
				if (zr * zr + zi * zi > 4.f)
					break;
				float zrtemp = zr * zr - zi * zi + cr;
				zi = 2.f * zr * zi + ci;
				zr = zrtemp;
			}
			pixelbuffer[y * FRAME_WIDTH + x] = ((max_iters - iters) >> 2) * 0x41 >> 1;
		}
	}	
}

void gfx_update()
{
	update(&state);
}

uint8_t* gfx_get_pixelbuffer()
{
	return pixelbuffer;
}
	
void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565)
{
	color_palette[color_idx]=color565;
}

uint16_t gfx_get_palettecolor(uint8_t color_idx)
{
	return color_palette[color_idx];
}

void     gfx_set_font(uint8_t* font_bpp1)
{
	font = font_bpp1;
}

// canvas functions
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y)
{
	return pixelbuffer[y*FRAME_WIDTH + x];
}

void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx)
{
	pixelbuffer[y*FRAME_WIDTH + x] = color_idx;
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

uint8_t* _pixelbuffer_location_ptr(uint16_t x,uint16_t y)
{
//	return pixelbuffer+y*FRAME_WIDTH+x;
	return &pixelbuffer[y*FRAME_WIDTH+x-1];
}

void gfx_draw_char(uint16_t x, uint16_t y, char ch, uint8_t color_idx)
{
	uint8_t* character_ptr = _char2fontbuffer(ch);

	for (int i=0;i<8;i++){
		// set tip on draw-position
		uint8_t* buffer_tip = _pixelbuffer_location_ptr(x,y++);
		uint8_t current_font_line = *(character_ptr++);
		uint8_t mask = 1;

		// unfold? let's not wait with crazy stuff
		for (int j=0;j<8;j++){
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
