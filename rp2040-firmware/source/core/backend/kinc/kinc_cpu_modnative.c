/// This is a sample of how to use the API directly without using the 6502 at all
#ifdef _MOD_NATIVE_

#ifdef __KINC__

#include <stdint.h>
#include <stdio.h>
#include "../../../ng_cpu.h"
#include "../../../ng_utils.h"
#include <kinc/system.h>
#include "kinc_ng.h"
#define UNDOCUMENTED

extern void mod_init();
extern void mod_update();

//#include "../../../../mod/source/mod.c"

#define RATE (1000/30)

long long last_ns=0;

void ng_cpu_init(void){
    mod_init();
}

int32_t timer=0;

void ng_cpu_update(void){
    if (!cpu_running){
        return;
    }

    timer += utils_millis();

    if (timer >= RATE){
        timer -= RATE;

        // these ng_cpu-calls actualy don't make sense, but I do it anyway ;)
        ng_cpu_before_tick();
        // tick the mod
        mod_update();
        ng_cpu_after_tick();
    }
}

#endif

#endif