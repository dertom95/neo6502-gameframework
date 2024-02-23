#ifdef PICO_NEO6502

#include "../../../ng_cpu.h"
#include "../cpu/wdc65C02cpu.h"

void ng_cpu_init(void){
    wdc65C02cpu_init();
    wdc65C02cpu_reset();    
}

void ng_cpu_update(){
    if (!cpu_running){
        return;
    }
    tick6502();
}

#endif