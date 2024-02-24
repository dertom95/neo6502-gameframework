#ifndef __NG_CPU_H_
#define __NG_CPU_H_

#include <stdbool.h>

extern bool cpu_running;

void ng_cpu_init(void);
void ng_cpu_start(void);
void ng_cpu_update(void);

void ng_cpu_before_tick(void);
void ng_cpu_after_tick(void);

#endif