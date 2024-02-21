#ifndef __NG_CPU_H_
#define __NG_CPU_H_

#include <stdbool.h>

bool cpu_running = false;

void ng_cpu_start(void) {
    cpu_running = true;
}


#endif