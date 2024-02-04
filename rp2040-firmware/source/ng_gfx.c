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
#include "ng_utils.h"
#include <assert.h>



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

// TODO: this must be done somewhere else in userspace
gfx_pixelbuffer_t initial_pixelbuffer={
	.x=0,
	.y=0,
	.width=320,
	.height=100,
};

ng_mem_block_t* render_queue[]={0};

uint8_t* font=NULL; 		  // 1bpp
gfx_pixelbuffer_t* pixelbuffer = NULL;

struct dvi_inst dvi0;
game_state_t state;

uint16_t __scratch_x("render") __attribute__((aligned(4))) core1_scanbuf[FRAME_WIDTH*2];


uint16_t gfx_color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}


static inline int clip(int x, int min, int max) {
	return x < min ? min : x > max ? max : x;
}

uint16_t* sp16 = NULL;

#define SPRITE_CONVERTED (1 << 0)



gfx_sprite_t sprites[MAX_SPRITES]={0};

uint8_t sprites_inuse_amount=0;
gfx_sprite_t* sprites_inuse_list[10]={0}; 

gfx_sprite_t* _gfx_sprite_find_free()
{
	for (int i=0;i<MAX_SPRITES;i++){
		gfx_sprite_t* sprite = &sprites[i];
		if (sprite->flags == 0){
			flags_set(sprite->flags, SPRITE_FLAG_INUSE);
			sprites_inuse_list[sprites_inuse_amount++]=sprite;
			return sprite;
		}
	}
	return NULL;
}

void* _gfx_copy_from_flash_to_ram(void* data,uint size){
	void* ram_data = malloc(size);
	assert(ram_data!=NULL && "could not allocate any more RAM");
	memcpy(ram_data,data,size);
	return ram_data;
}

gfx_sprite_t* gfx_sprite_create_from_tilesheet(int16_t x,int16_t y, gfx_tilesheet_t* ts)
{
	gfx_sprite_t* sprite = _gfx_sprite_find_free();
	if (sprite == NULL){
		return NULL;
	}
	sprite->tilesheet = ts;
	sprite->x=x;
	sprite->y=y;

	if (ts->tilesheet_data_ram == NULL){
		// we need to copy sprites to RAM!
		uint8_t* ts_ram_data = _gfx_copy_from_flash_to_ram(ts->tilesheet_data_flash,ts->header.size);
		ts->tilesheet_data_ram = ts_ram_data;
	}

	gfx_sprite_set_tileid(sprite,0);

	return sprite;
}

void    gfx_sprite_set_tileid(gfx_sprite_t* sprite, uint8_t tile_id)
{
	assert(tile_id < sprite->tilesheet->tile_amount && "exceeded tile-id");
	assert(sprite->tilesheet!=NULL && "no tilesheet set for sprite");
	assert(sprite->tilesheet->tilesheet_data_ram!=NULL && "tilesheet data not copied to RAM");
	
	gfx_tilesheet_t* ts = sprite->tilesheet;
	uint size_per_tile = (ts->tile_width*ts->tile_height);
	uint8_t* tile_ptr = ts->tilesheet_data_ram + tile_id * size_per_tile;
	sprite->tile_ptr = tile_ptr;
	sprite->tile_id = tile_id;
}

void __not_in_flash_func(gfx_tile_set_color)(uint8_t tX,uint8_t tY,uint8_t col)
{
    int16_t _x = tX * 8;
	int16_t _y = tY * 8;
	if (_x < 0 || _y < 0 || _x>pixelbuffer->width || _y>pixelbuffer->height){
		return;
	}
	uint8_t width = min(8, pixelbuffer->width-tX);
	uint8_t height = min(8, pixelbuffer->height-tY);
	for (int y=_y,yEnd=(_y+width);y<yEnd;y++){
        for (int x=_x,xEnd=(_x+height);x<xEnd;x++){
            gfx_draw_pixel(x,y,col);
        }
    }
}


// SCANLINE-RENDERER
static void __not_in_flash_func(render_scanline)(uint16_t *pixbuf, uint y, const game_state_t *gstate) {
	memset(pixbuf,0,320*sizeof(uint16_t));
	uint16_t* write_buf = pixbuf;
	{
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
				count = min(FRAME_WIDTH-pixelbuffer->x, pixelbuffer->width);
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
	}

	for (int i = 0; i < sprites_inuse_amount; i++){
		gfx_sprite_t* sprite = sprites_inuse_list[i];

		gfx_tilesheet_t* ts = sprite->tilesheet;
		if (sprite->y > y || (sprite->y+ts->tile_height)<y){
			continue;
		}

		uint8_t count = min(sprite->tilesheet->tile_width,FRAME_WIDTH-sprite->x);
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

uint8_t* _pixelbuffer_location_ptr(uint16_t x,uint16_t y)
{
	assert(pixelbuffer!=NULL);
	assert(x>0 || x < pixelbuffer->width);
	assert(y>0 || y < pixelbuffer->height);

	uint8_t* result = pixelbuffer->mem.data;
	result += x + y * pixelbuffer->width;
	return result;
}



void gfx_init()
{
	gfx_pixelbuffer_create(SEGMENT_GFX_DATA,&initial_pixelbuffer);
	gfx_pixelbuffer_set_active(&initial_pixelbuffer);

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
}

void gfx_update()
{
}


void     gfx_set_palettecolor(uint8_t color_idx, uint16_t color565)
{
	color_palette[color_idx]=color565;
}

void gfx_set_palette_from_assset(uint8_t asset_id, uint16_t fill_unused_with)
{
	gfx_palette_t* palette = asset_get_pointer(asset_id);
	memcpy(color_palette, palette+1, palette->color_amount * sizeof(uint16_t));
	if (palette->color_amount<256){
		uint8_t dif = 256 - palette->color_amount;
		uint16_t* tip = color_palette + palette->color_amount;
		while (dif-->0){
			*(tip++) = fill_unused_with;
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
	uint8_t* font = asset_get_pointer(asset_id);
	gfx_set_font(font);
}

// canvas functions
uint8_t  gfx_get_pixel(uint16_t x, uint16_t y)
{
	uint8_t* pixelbuffer_data = _pixelbuffer_location_ptr(x,y);
	return *pixelbuffer_data;
}

void     gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t color_idx)
{
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
	if (   x < 0 || x > pixelbuffer->width 
		|| y < 0 || y > pixelbuffer->height
	){
		return;
	}

	uint8_t* character_ptr = _char2fontbuffer(ch);

	uint8_t width = min(8, pixelbuffer->width-x);

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
	assert(ng_mem_segment_space_left(size) && "gfx_create_pixelbuffer: create size");
	
	bool success = ng_mem_allocate(segment_id, size, MEM_USAGE_PIXELBUFFER, &initial_data->mem );
	return success;
}

void gfx_pixelbuffer_set_active(gfx_pixelbuffer_t* pxbuffer)
{
	pixelbuffer = pxbuffer;
}

gfx_pixelbuffer_t* gfx_pixelbuffer_get_current(void)
{
	return pixelbuffer;
}