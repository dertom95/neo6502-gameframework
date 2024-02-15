#if 1

#ifdef __KINC__

#include <stdint.h>
#include <stdio.h>
#include "../../../ng_cpu.h"
#include "kinc_ng.h"
#define UNDOCUMENTED
//#include "../cpu/fake65c02.h"
#include "../cpu/fake65c02.h"
#include "../../../core/memory.h"

uint32_t tickscpu;

uint8 read6502(ushort _address)
{
    uint8_t data = memory_read_data(_address);  
    return data;
}

void write6502(ushort _address, uint8 value)
{
    memory_write_data(_address,value);
}

// uint8 read6502(ushort _address)
// {
//     uint8_t data = mem[_address];  
//     return data;
// }

// void write6502(ushort _address, uint8 value)
// {
//     mem[_address]=value;
// }


void ng_cpu_init(void){
    reset6502();
}

void ng_cpu_update(){
    //step6502();
    exec6502(STEPS_6502_CALL);
    //printf("TICK: %d\n",tickscpu);
    tickscpu+=STEPS_6502_CALL;
}

#endif

#endif