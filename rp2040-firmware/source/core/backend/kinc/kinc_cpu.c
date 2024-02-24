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

#define STEP_NS 1000

long long diff = 0;
uint32_t ns_timer = 0;

extern uint16_t* mm_cycle_ticks;

uint32_t counter2 = 0;
void ng_cpu_update(void){
    if (!cpu_running){
        return;
    }
    
    long long tick = get_ns();
    diff = tick - last_ns;
    last_ns = tick;
    ns_timer += diff;

    if (ns_timer > STEP_NS){
        ns_timer -= STEP_NS;
        ng_cpu_before_tick();
        step6502();
        ng_cpu_before_tick();
    }

}

#endif

#endif