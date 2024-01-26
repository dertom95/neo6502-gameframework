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

/// <summary>
/// 64k RAM
/// </summary>
uint8_t mem[MEMORY_SIZE];

// address and data registers
uint16_t address;
uint8_t  data;

uint16_t write_address;
uint8_t write_data;

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

}
uint8_t memory_read_data() {
  if (address >= MEMORY_MAP_START && address <= MEMORY_MAP_END){
    switch (address) {
        case MM_KEYSET : {
          return data = last_pressed_keycode;
        }
    }
  } else {
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