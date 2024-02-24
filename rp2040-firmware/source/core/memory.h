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
//extern uint8_t write_data;

void memory_write_data(uint32_t address,uint8_t data);
uint8_t memory_read_data(uint32_t address);
void memory_init();
uint8_t call_function(); // generated


#ifdef __cplusplus
}
#endif

#endif

