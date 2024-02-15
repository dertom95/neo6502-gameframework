#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../test/gen_prg1.h"
#include "../source/core/backend/cpu/fake65c02.h"

uint8_t mem[0xffff];

const uint8_t vectors_bin[] = {
  0x00, 0x00, // NMI
	//0x20, 0xFC, // RESET
	0x00, 0x10, // RESET
	0x50, 0xFE  // IRQ/BRK
};

uint8 read6502(ushort _address)
{
    uint8_t data = mem[_address];  
    return data;
}

void write6502(ushort _address, uint8 value)
{
    mem[_address]=value;
}

int main(){
    memcpy(&mem[0x1000],bin2c_prg1_bin,sizeof(bin2c_prg1_bin));
    memcpy(&mem[0xfffa],vectors_bin,6);

    reset6502();
    int step = 0;
    while(1){
        step6502();
        printf("step:%06d counter:%03d\n",step,mem[0x2000]);
        step++;
    }
}