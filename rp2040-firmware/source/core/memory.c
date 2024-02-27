#ifdef __cplusplus
extern "C" {
#endif

// 
// Author: Rien Matthijsse
// 
#include "memory.h"
#include "roms.h"
#include "../ng_defines.h"
#include "../api/gen/ng_api.h"
#include "../ng_io.h"
#include "../ng_gfx.h"
#include "../api/gen/memory_call_function.c"
#include "../ng_utils.h"
#include <assert.h>




/// <summary>
/// 64k RAM
/// </summary>


uint8_t gfx_ram[GFX_MEMORY_SIZE] __attribute__((aligned(16)));
uint8_t mem[CPU6502_MEMORY_SIZE] __attribute__((aligned(16)));

// address and data registers
uint32_t last_address;
uint8_t last_data;

// memorymapped variables
uint16_t* mm_cycle_ticks;
uint16_t* mm_ms_delta;

int16_t* mm_mouse_x=NULL;
int16_t* mm_mouse_y=NULL;
uint8_t* mm_mouse_btn_state=NULL;
int8_t*  mm_mouse_wheel=NULL;
uint8_t* mm_keyboard_last_pressed_keycode = NULL;
char*    mm_keyboard_last_pressed_char = NULL;



/// <summary>
/// initialise memory
/// </summary>
void memory_init() {
  id_init(64);
  last_address = 0UL;
  last_data = 0;

  uint8_t seg_id = ng_mem_segment_create(gfx_ram,GFX_MEMORY_SIZE);
  assert(seg_id==SEGMENT_GFX_DATA && "segment id mismatch! GFX_MEMORY needs to be the first segment to be created! otherwise alter #defines");
  seg_id = ng_mem_segment_create(mem,CPU6502_MEMORY_SIZE);
  assert(seg_id==SEGMENT_6502_MEM && "segment id mismatch! CPU6502_MEMORY_SIZE needs to be the 2nd segment to be created! otherwise alter #defines");

  mm_cycle_ticks = (uint16_t*)&mem[MM_CYCLE_TICKS];
  *mm_cycle_ticks=0;


  mm_keyboard_last_pressed_char = MEM6502_CHPTR(MM_LAST_KB_CHAR);
  *mm_keyboard_last_pressed_char=0;
  mm_keyboard_last_pressed_keycode = MEM6502_U8PTR(MM_LAST_KB_KEYCODE);
  *mm_keyboard_last_pressed_keycode=0;
  
  mm_ms_delta = (uint16_t*)&mem[MM_MS_DELTA];
  *mm_ms_delta=0;
  mm_mouse_btn_state = MEM6502_U8PTR(MM_MOUSE_BTN);
  *mm_mouse_btn_state = 0;
  mm_mouse_wheel = MEM6502_I8PTR(MM_MOUSE_WHEEL);
  *mm_mouse_wheel = 0;
  mm_mouse_x = MEM6502_I16PTR(MM_MOUSE_X);
  *mm_mouse_x=0;
  mm_mouse_y = MEM6502_I16PTR(MM_MOUSE_Y);
  *mm_mouse_y=0;


  // lets install some ROMS
  // if (loadROMS()) {
  //   Serial.println("ROMs installed");
  // }
}

void __not_in_flash_func(memory_write_data)(uint32_t address,uint8_t data) {
  last_address=address;
  if (address >= MEMORY_MAP_FUNC_START && address <= MEMORY_MAP_FUNC_END){
    // memory map
  }
  if (address >= MEMORY_TILEAREA_BEGIN && address <= MEMORY_TILEAREA_END){
      // simple blocky tilemap
      uint16_t addressTilemapSpace = address - MEMORY_TILEAREA_BEGIN;
      gfx_tile_set_color(addressTilemapSpace % MEMORY_TILES_WIDTH, addressTilemapSpace / MEMORY_TILES_WIDTH, data);
  }
  else {
    mem[address]=data;
  } 
}


uint8_t __not_in_flash_func(memory_read_data)(uint32_t address) {
  last_address = address;
  // memory map
  if (address >= MEMORY_MAP_FUNC_START && address <= MEMORY_MAP_FUNC_END){
    switch (address) {
        case MM_FUNC_CALL:{
          return call_function();
        }
        default:
          ASSERT_STRICT(false && "ACCESSING MEMORYMAPPED AREA-");
          return mem[address];  
    }
  } 
  else if (address >= MEMORY_TILEAREA_BEGIN && address <= MEMORY_TILEAREA_END){
    // TILEMAP-Reader
    return 0x95;
  }
  
  else {
    return last_data = mem[address];
  }
}


// /// <summary>
// /// read a byte from memory
// /// </summary>
// uint8_t readmemory() {
//   if (address > )
//   data = mem[address];
// }

// /// <summary>
// /// store a byte into memory
// /// </summary>
// void writememory() {
//   if ((0xA000 <= address && address <= 0xCFFF) || (0xE000 <= address && address <= 0xFFF9)) { // exclude writing ROM
//     //Serial.printf("access violation [%04X]\n", address);
//   }
//   else{
//     mem[address] = data;
//     write_address=address;
//     write_data = data;
//   }
// }

// void mem_set_byte(uint16_t address,uint8_t data){

// }

// void mem_set_word(uint16_t address,uint16_t data) {

// }

#ifdef __cplusplus
}
#endif