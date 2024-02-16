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

#include <kinc/system.h>

#include <time.h>

long long get_ns() {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    return start_time.tv_sec * 1000000000LL + start_time.tv_nsec;
}

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

long long last_ns=0;

void ng_cpu_init(void){
    last_ns = get_ns();
    reset6502();
}

#define STEP_NS 25000

uint16_t counter2 = 0;
void ng_cpu_update(){
    long long tick = get_ns();
    long long diff = tick - last_ns;
    last_ns=tick;

    if (counter2++>8){
        step6502();
        counter2=0;
        tickscpu+=1;
    }





    //exec6502(STEPS_6502_CALL);
    //printf("TICK: %d\n",tickscpu);
}

#endif

#endif