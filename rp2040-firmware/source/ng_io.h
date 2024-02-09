#ifndef __NG_USB_H__
#define __NG_USB_H__

#include<stdint.h>
#include<stdbool.h>


extern int16_t mouse_x;
extern int16_t mouse_y;
extern uint8_t mouse_btn_state;
extern int8_t mouse_wheel;
extern uint8_t keyboard_last_pressed_keycode;
extern char keyboard_last_pressed_key;

void io_init(void);
void io_update(void);


bool io_keyboard_connected(void);
bool io_mouse_connected(void);

// key just pressed
bool io_keyboard_is_pressed(uint8_t keycode);
// key is hold down
bool io_keyboard_is_down(uint8_t keycode);
// key is just released
bool io_keyboard_is_released(uint8_t keycode);
// converts keycode to ascii
uint8_t io_keycode_to_ascii(uint8_t keycode);



#endif