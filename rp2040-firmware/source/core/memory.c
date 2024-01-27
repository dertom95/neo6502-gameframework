#ifdef __cplusplus
extern "C" {
#endif

// 
// Author: Rien Matthijsse
// 
#include "memory.h"
#include "roms.h"
#include "../api/ng_api.h"
#include "../ng_io.h"
#include "../ng_gfx.h"

/// <summary>
/// 64k RAM
/// </summary>
uint8_t mem[MEMORY_SIZE];

// address and data registers
uint16_t address;
uint8_t  data;

/// <summary>
/// initialise memory
/// </summary>
void initMemory() {
  address = 0UL;
  data = 0;

  // lets install some ROMS
  // if (loadROMS()) {
  //   Serial.println("ROMs installed");
  // }
}

void memory_write_data(uint8_t data) {
  if (address >= MEMORY_MAP_START && address <= MEMORY_MAP_END){
    // memory map
  }
  if (address >= MEMORY_TILEAREA_START && address <= MEMORY_TILEAREA_END){
      // simple blocky tilemap
      uint16_t addressTilemapSpace = address - MEMORY_TILEAREA_START;
      gfx_tile_set_color(addressTilemapSpace % MEMORY_TILES_WIDTH, addressTilemapSpace / MEMORY_TILES_WIDTH, data);
  } 
  else {
    mem[address]=data;
  } 
}

#define CASE_16BIT(NAME,variable) \
        case NAME: return data = (variable & 0x00ff); \
        case NAME+1: return data = (variable & 0xff00)>>8; \

#define CASE_8BIT(NAME,variable) \
        case NAME: return data = (variable);

uint8_t memory_read_data() {
  // memory map
  if (address >= MEMORY_MAP_START && address <= MEMORY_MAP_END){
    switch (address) {
        //CASE_8BIT(MM_KEYSET,last_pressed_key)
        case (MM_KEYSET): {
          return data = (keyboard_last_pressed_key);
        }
        CASE_16BIT(MM_MOUSE_X,mouse_x)
        CASE_16BIT(MM_MOUSE_Y,mouse_y)
        CASE_8BIT(MM_MOUSE_BTN, mouse_btn_state)
        CASE_8BIT(MM_MOUSE_WHEEL,mouse_wheel);
        default:
          //unknown
          return data = 0x95;  
    }
  } 
  else if (address >= MEMORY_TILEAREA_START && address <= MEMORY_TILEAREA_END){
    // TILEMAP-Reader
    return 0x95;
  }
  
  else {
    return data = mem[address];
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