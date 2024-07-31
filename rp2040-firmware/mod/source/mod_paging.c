
#include <stdint.h>
#include <stdio.h>


#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include <gen/ng_api.c>
#endif

#include "export/assets.h"
#include <ng_api_shared.h>

volatile uint16_t* mx = NULL;
volatile uint16_t* my = NULL;
volatile uint8_t* mbtn = NULL;
volatile uint8_t* pxbuf = NULL;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PX (320/SCREEN_WIDTH)
#define PY (240/SCREEN_HEIGHT)

gfx_pixelbuffer_t pixelbuffer = {
    .width=SCREEN_WIDTH,
    .height=SCREEN_HEIGHT,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(PX,PY),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

#define MOUNT_POS 0x6000

#define TICK_RATE (1000/30)
volatile uint16_t* ms_delta = NULL;

uint8_t px_width;
uint8_t px_height;

uint16_t seed;
uint8_t pxb_mount_id;

int random() {
    seed = seed * 1103515245 + 12345;
    return (unsigned int)(seed / 65536) % 32768;
}

#define TXT_LEN 30
char txt[TXT_LEN];

int16_t x=0;

void draw_cube(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t colidx)
{
    for(uint16_t y=_y,yEnd=_y+height;y<yEnd;y+=1){
        for (uint16_t x=_x,xEnd=_x+width;x<xEnd;x++){
            gfx_draw_pixel(x,y, colidx);
        }
    }
}

void draw_cube_mem(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t colidx)
{
    for(uint16_t y=_y,yEnd=_y+height;y<yEnd;y+=1){
        gfx_mount_set_page(pixelbuffer.mount_id,(uint8_t)y);
#ifdef _MOD_NATIVE_    
        pxbuf = (uint8_t*)MEMPTR(MOUNT_POS);        
#endif        
        for (uint16_t x=_x,xEnd=_x+width;x<xEnd;x++){
            *(pxbuf+x)=colidx;
        }
    }
}

int mod_init(){
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mx =  (uint16_t*)MEMPTR(MM_MOUSE_X);
    my =  (uint16_t*)MEMPTR(MM_MOUSE_Y);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);
    pxbuf = (uint8_t*)MEMPTR(MOUNT_POS);

    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    draw_cube(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,COL_LIGHTGREY);

    gfx_draw_pixel(0,0,COL_RED);
    gfx_draw_pixel(SCREEN_WIDTH-1,0,COL_GREEN);
    gfx_draw_pixel(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,COL_GREEN);
    gfx_draw_pixel(0,SCREEN_HEIGHT-1,COL_RED);

    draw_cube(SCREEN_WIDTH/2-10,SCREEN_HEIGHT/2-10,20,20, COL_ORANGE);
    gfx_pixelbuffer_mount(&pixelbuffer,MOUNT_POS,320);

#ifdef _MOD_NATIVE_    
    // memory mappings needs to be queried after they are mounted in _MOD_NATIVE_
    pxbuf = (uint8_t*)MEMPTR(MOUNT_POS);
#endif

    for (uint8_t row=0;row<240;row++){
        gfx_mount_set_page(pixelbuffer.mount_id,row);
#ifdef _MOD_NATIVE_    
        pxbuf = (uint8_t*)MEMPTR(MOUNT_POS);        
#endif
        for (int i=0;i<320;i++){
            *(pxbuf+i)=(uint8_t)row;
        }
    }
    gfx_draw_text(5,5,txt,COL_ORANGE);

    gfx_renderqueue_add_id(pixelbuffer.obj_id);

    gfx_renderqueue_apply();

    flags_unpack_4_4(pixelbuffer.pixel_size,px_width,px_height);
    int a=0;
}

uint16_t last_x =0;
uint16_t col = 0;
void mod_update() {
    // TODO: implement some kind of sleep
    if (*ms_delta<TICK_RATE)
    {
        return;
    }

    col++;
    if (col%16==0){
        col=1;
    }
    draw_cube_mem(last_x,SCREEN_HEIGHT/2-10,20,20, col);
    last_x++;
    if (last_x==240){
        last_x=0;
    }
    
    int16_t dt = *ms_delta;
    *ms_delta=0;

    bool changed = false;

   
    //flags_set(pixelbuffer.flags,PXBFLAG_DIRTY);
}

#ifndef _MOD_NATIVE_
int main(){
    mod_init();

    while(1){
        mod_update();
    }

    return 0;
}
#endif