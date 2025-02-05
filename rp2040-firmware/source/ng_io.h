#ifndef __NG_IO_H__
#define __NG_IO_H__

#include<stdint.h>
#include<stdbool.h>

#include "api/ng_config.h"

typedef struct keyboard_environment_t {
  uint8_t keyboardmapping_amount;
  keyboard_mapping_t* keyboardmappings;
} keyboard_environment_t;

typedef struct gamepad_registration_t gamepad_registration_t;

typedef void (*gamepad_function_t)(gamepad_registration_t*, void*);



extern bool _keyboard_connected;
extern bool _mouse_connected;

void io_init(void);
void io_before_tick(void);
void io_after_tick(void);

void io_backend_init(void);
void io_backend_before_tick(void);
void io_backend_after_tick(void);

bool io_keyboard_connected(void);
bool io_mouse_connected(void);

// key just pressed
/*api:2:1*/bool io_keyboard_is_pressed(uint8_t keycode);
// key is hold down
/*api:2:2*/bool io_keyboard_is_down(uint8_t keycode);
// key is just released
/*api:2:3*/bool io_keyboard_is_released(uint8_t keycode);

/// @brief register keyboardmappings to the system
/// @param address 
/// @param amount 
/*api:2:4*/void io_keyboardmapping_register(keyboard_mapping_t* address, uint8_t amount);

/// @brief Unregisters the current keyboard mapping!
/*api:2:5*/void   io_keyboardmapping_unregister(void);

// converts keycode to ascii
uint8_t io_keycode_to_ascii(uint8_t keycode);



#endif