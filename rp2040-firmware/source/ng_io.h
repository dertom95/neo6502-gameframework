#ifndef __NG_USB_H__
#define __NG_USB_H__

#include<stdint.h>
#include<stdbool.h>

void usb_init(void);
void usb_update(void);

bool keyboard_connected(void);
bool mouse_connected(void);

// key just pressed
bool keyboard_is_pressed(uint8_t keycode);
// key is hold down
bool keyboard_is_down(uint8_t keycode);
// key is just released
bool keyboard_is_released(uint8_t keycode);


#endif