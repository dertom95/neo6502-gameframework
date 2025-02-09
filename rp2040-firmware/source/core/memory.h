// 
// Author: Rien Matthijsse
// 

#ifndef _MEMORY_h
#define _MEMORY_h

#include <stdint.h>
#include "../api/ng_config.h"
#include "../ng_mem.h"


#ifdef __cplusplus
extern "C" {
#endif



extern uint8_t  mem[];
extern uint32_t last_address;
extern uint8_t  last_data;
extern uint16_t* mm_cycle_ticks;
extern uint16_t* mm_ms_delta;
extern int16_t* mm_mouse_x;
extern int16_t* mm_mouse_y;
extern uint8_t* mm_mouse_btn_state;
extern int8_t*  mm_mouse_wheel;
extern uint8_t* mm_keyboard_last_pressed_keycode;
extern char*    mm_keyboard_last_pressed_char;
extern gamepad_t* mm_gamepad;
extern uint8_t* mm_gamepad_info;

extern gamepad_state_t* mm_gamepad_down;
extern gamepad_state_t* mm_gamepad_pressed;
extern gamepad_state_t* mm_gamepad_released;



#define MEM6502_CHPTR(pos) ((char*)&mem[pos])
#define MEM6502_U16PTR(pos) ((uint16_t*)&mem[pos])
#define MEM6502_U8PTR(pos) ((uint8_t*)&mem[pos])
#define MEM6502_I16PTR(pos) ((int16_t*)&mem[pos])
#define MEM6502_I8PTR(pos) ((int8_t*)&mem[pos])

//extern uint8_t write_data;


uint8_t* memory_resolve_address(uint16_t address);
void memory_write_data(uint16_t address,uint8_t data);
uint8_t memory_read_data(uint16_t address);
void memory_init();
uint8_t call_function(); // generated

#ifdef __cplusplus
}
#endif

#endif

