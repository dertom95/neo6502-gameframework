// 
// Author: Rien Matthijsse
// 

#ifndef _MEMORY_h
#define _MEMORY_h

#include <stdint.h>
#include "../api/ng_config.h"


#ifdef __cplusplus
extern "C" {
#endif


extern uint8_t  mem[];
extern uint16_t address;
extern uint8_t  data;
extern uint16_t write_address;
extern uint8_t write_data;

void memory_write_data(uint8_t data);
uint8_t memory_read_data();

//externally supplied functions
// void initMemory();

// void readmemory();

// void writememory();

#ifdef __cplusplus
}
#endif

#endif

