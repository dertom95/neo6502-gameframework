#ifndef __NG_IO_H__
#define __NG_IO_H__

#include<stdint.h>
#include<stdbool.h>


extern int16_t mouse_x;
extern int16_t mouse_y;
extern uint8_t mouse_btn_state;
extern int8_t mouse_wheel;
extern uint8_t keyboard_last_pressed_keycode;
extern char keyboard_last_pressed_key;
extern bool _keyboard_connected;
extern bool _mouse_connected;

void io_init(void);
void io_update(void);

void io_backend_init(void);
void io_backend_update(void);

bool io_keyboard_connected(void);
bool io_mouse_connected(void);

// key just pressed
/*api:2:1*/bool io_keyboard_is_pressed(uint8_t keycode);
// key is hold down
/*api:2:2*/bool io_keyboard_is_down(uint8_t keycode);
// key is just released
/*api:2:3*/bool io_keyboard_is_released(uint8_t keycode);
// converts keycode to ascii
uint8_t io_keycode_to_ascii(uint8_t keycode);



#endif