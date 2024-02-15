#if 0

#not working

#ifdef __KINC__ 

#include <stdint.h>
#include "../../../ng_cpu.h"
#define CHIPS_IMPL
#include "../cpu/m6502.h"
#include "../../../core/memory.h"

uint32_t tickscpu;
m6502_t cpu;
uint64_t pins;

void ng_cpu_init(void){
    pins = m6502_init(&cpu, &(m6502_desc_t){});
}

void ng_cpu_update(){
    pins = m6502_tick(&cpu, pins);

    // run the CPU emulation for one tick
    // extract 16-bit address from pin mask
    uint16_t address = M6502_GET_ADDR(pins);
    // perform memory access
    if (pins & M6502_RW) {
        // a memory read
        uint8_t data = memory_read_data(address);    
        M6502_SET_DATA(pins, data);
    }
    else {
        // a memory write
        uint8_t data = M6502_GET_DATA(pins);
        memory_write_data(address,data);
    }
}

#endif

#endif