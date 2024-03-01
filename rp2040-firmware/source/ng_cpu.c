#ifndef __NG_CPU_H_
#define __NG_CPU_H_

#include <stdbool.h>
#include "core/memory.h"

bool cpu_running = false;
uint32_t tickscpu;

void ng_cpu_start(void) {
    cpu_running = true;
}

void ng_cpu_before_tick(void)
{
    *mm_cycle_ticks++;
    tickscpu+=1;
}
void ng_cpu_after_tick(void)
{
}

#endif