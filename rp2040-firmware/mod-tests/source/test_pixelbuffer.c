#include "tests.h"

#include <stdlib.h>

#define ng_ms_delta *MEMPTR(MM_MS_DELTA)
#define _mouse_x *MEMPTR(MM_MOUSE_X)
#define _mouse_y *MEMPTR(MM_MOUSE_Y)
#define _mouse_pressed *MEMPTR(MM_MOUSE_BTN_PRESSED)
#define _mouse_released *MEMPTR(MM_MOUSE_BTN_RELEASED)
#define _mouse_down *MEMPTR(MM_MOUSE_BTN)

gamepad_state_t* gp0_pressed;

#define KEY_F1 (1 << 0)
#define KEY_F2 (1 << 1)
#define KEY_F3 (1 << 2)
#define KEY_F4 (1 << 3)
#define KEY_UP (1 << 4)
#define KEY_DOWN (1 << 5)
#define KEY_LEFT (1 << 6)
#define KEY_RIGHT (1 << 7)


typedef struct TestContext {
    gfx_pixelbuffer_t pxb;
    keyboard_mapping_t keyb;
} TestContext;

TestContext* ctx = NULL;

void draw_debug(){
    gfx_debug_drawinfo_pixelbuffer(0,0,&ctx->pxb,COL_BLACK,COL_WHITE);
    gfx_debug_drawinfo_keyboard(0,16,&ctx->keyb,COL_BLACK,COL_GREY_7_LIGHT);
    gfx_debug_drawinfo_mouse(0,40,COL_BLACK,COL_GREY_4);
    gfx_debug_drawinfo_gamepad(0,56,0,COL_BLACK,COL_GREY_7_LIGHT);
}


void test1_init() {
    ctx = malloc(sizeof(TestContext));

    *ctx = (TestContext){0};
    ctx->pxb = (gfx_pixelbuffer_t) {
        .pixel_size=flags_pack_4_4(1,1),
        .width = 320 / 2,
        .height = 240 / 2
    };

    gp0_pressed = MEMPTR(MM_GAMEPAD1_STATE_PRESSED);

    ctx->keyb = (keyboard_mapping_t){
        .flags = KEYBMAP_FLAG_SCAN_ALL,
        .keycodes = {
            HID_KEY_F1,
            HID_KEY_F2,
            HID_KEY_F3,
            HID_KEY_F4,
            HID_KEY_ARROW_UP,
            HID_KEY_ARROW_DOWN,
            HID_KEY_ARROW_LEFT,
            HID_KEY_ARROW_RIGHT,
        }
    };

    io_keyboardmapping_register(&ctx->keyb,1);

    gfx_pixelbuffer_create(&ctx->pxb);
    uint8_t obj = ctx->pxb.obj_id;

    gfx_pixelbuffer_set_active(&ctx->pxb);

    gfx_pixelbuffer_t data = ctx->pxb;
    ng_debug_value(obj,data.obj_id);

    gfx_renderqueue_wipe();
    ng_debug_pointer(&ctx->pxb,1);
    ng_debug_value(obj,ctx->pxb.obj_id);
    gfx_renderqueue_add_id(data.obj_id);
    gfx_renderqueue_add_id(pxb_instructions.obj_id);
    gfx_renderqueue_apply();

    draw_debug();
}


#define FILLMODE_DEFAULT 0
#define FILLMODE_RANDOM  1
void fill_pixelbuffer(uint8_t mode){
    for (int y=0,yEnd=ctx->pxb.height; y < yEnd; y++){
        for (int x=0,xEnd=ctx->pxb.width; x < xEnd; x++){
            uint8_t col;
            if (mode == 0){
                col = (x+y) % 254;
            }
            else if (mode==1){
                col = utils_random_uint16() % 254;
            }
            gfx_draw_pixel(x,y,col);
        }
    }
}

void move_pixelbuffer(int8_t delta_x,int16_t delta_y){
    ctx->pxb.x+=delta_x;
    ctx->pxb.y+=delta_y;
    gfx_pixelbuffer_apply_data(&ctx->pxb);
}

void resize_pixelbuffer(int8_t delta_x,int8_t delta_y){
    uint8_t px,py;
    flags_unpack_4_4(ctx->pxb.pixel_size,px,py);
    px = clamp(px+delta_x,0,8);
    py = clamp(py+delta_y,0,8);
    ctx->pxb.pixel_size=flags_pack_4_4(px,py);
    gfx_pixelbuffer_apply_data(&ctx->pxb);
}

typedef enum ActionState {
    as_none,
    as_default_action,
    as_shift,
} ActionState;

ActionState action_state = as_none;
uint16_t last_mx=0,last_my=0;

void test1_update() {
    io_before_tick();

    uint8_t key_pressed = ctx->keyb.key_pressed;
    uint8_t key_down = ctx->keyb.key_down;

    uint8_t mouse_btn_state_pressed = _mouse_pressed;
    bool mouse_left_pressed = bit_is_set_all(mouse_btn_state_pressed,MOUSE_BTN_LEFT);
    bool mouse_middle_pressed = bit_is_set_all(mouse_btn_state_pressed, MOUSE_BTN_MIDDLE);
    bool mouse_right_pressed = bit_is_set_all(mouse_btn_state_pressed, MOUSE_BTN_RIGHT);

    bool gp_left = bit_is_set_all(gp0_pressed->controls,GP_D_LEFT);
    bool gp_right = bit_is_set_all(gp0_pressed->controls,GP_D_RIGHT);
    bool gp_up = bit_is_set_all(gp0_pressed->controls,GP_D_UP);
    bool gp_down = bit_is_set_all(gp0_pressed->controls,GP_D_DOWN);

    if (mouse_left_pressed){
        int a=0;
    }

//    bool key_ctrl_down = io_keyboard_is_down(HID_KEY_CONTROL_LEFT);
    bool key_ctrl_down = io_keyboard_is_down(HID_KEY_A);
    bool consumed = false;
    int8_t md_x = _mouse_x - last_mx;
    int8_t md_y = _mouse_y - last_my;
    last_mx = _mouse_x;
    last_my = _mouse_y;

    if (!key_ctrl_down){
        if (action_state != as_default_action){
            action_state = as_default_action;
            draw_instructions("F1:default-draw F2:random-draw\nArrows:move");
        }
        // draw
        if (flags_isset(key_down,KEY_F1)){
            ng_debug_value(1,2);
            fill_pixelbuffer(FILLMODE_DEFAULT);
        }  
        else if (flags_isset(key_pressed,KEY_F2)){
            fill_pixelbuffer(FILLMODE_RANDOM);
        }

        // resize
        if (flags_isset(key_pressed,KEY_F3)){
            resize_pixelbuffer(-1,-1);
        }
        else if (flags_isset(key_pressed,KEY_F4)){
            resize_pixelbuffer(1,1);
        }


        // arrows
        if (flags_isset(key_pressed,KEY_UP) || gp_up){
            move_pixelbuffer(0,-1);
        }
        if (flags_isset(key_pressed,KEY_DOWN) || gp_down){
            move_pixelbuffer(0,1);
        }
        if (flags_isset(key_pressed,KEY_LEFT) || mouse_left_pressed || gp_left){
            move_pixelbuffer(-1,0);
        }
        if (flags_isset(key_pressed,KEY_RIGHT) || mouse_right_pressed || gp_right){
            move_pixelbuffer(1,0);
        }
    } else {
        if (action_state != as_shift){
            action_state = as_shift;
            draw_instructions("CTRL MENU");
        }
        // arrows
        if (flags_isset(key_down,KEY_UP)){
            move_pixelbuffer(0,-1);
        }
        if (flags_isset(key_down,KEY_DOWN)){
            move_pixelbuffer(0,1);
        }
        if (flags_isset(key_down,KEY_LEFT)){
            move_pixelbuffer(-1,0);
        }
        if (flags_isset(key_down,KEY_RIGHT)){
            move_pixelbuffer(1,0);
        }  
        if (md_x!=0 || md_y!=0){
            move_pixelbuffer(md_x,md_y);
        }      
    }

    draw_debug();
}

void test1_clean() {

}