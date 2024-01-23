// 
// Author: Rien Matthijsse
// 

#ifndef _MEMORY_h
#define _MEMORY_h

#include <stdint.h>

#define MEMORY_SIZE  0x10000 // 64k

#define KBD   0xd010
#define DSP   0xd012

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t  mem[];
extern uint16_t address;
extern uint8_t  data;
extern uint16_t write_address;
extern uint8_t write_data;

//externally supplied functions
void initMemory();

void readmemory();

void writememory();

#ifdef __cplusplus
}
#endif

#endif

