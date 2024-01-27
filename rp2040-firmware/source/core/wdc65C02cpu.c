#include "wdc65C02cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "pico/sem.h"

#include <string.h>

#include "../ng_io.h"
#include "../api/ng_api.h"

#ifdef CHIPS_IMPL
# include <string.h>
# ifndef CHIPS_ASSERT
#  include <assert.h>
#  define CHIPS_ASSERT(c) assert(c)
# endif

# ifdef PICO_NEO6502
#  define _GPIO_MASK       (0xFF)
#  define _GPIO_SHIFT_BITS (0)
#  define _OE1_PIN         (8)
#  define _OE2_PIN         (9)
#  define _OE3_PIN         (10)
#  define _RW_PIN          (11)
#  define _CLOCK_PIN       (21)
#  define _AUDIO_PIN       (20)
#  define _RESET_PIN       (26)
#  define _IRQ_PIN         (25)
#  define _NMI_PIN         (27)
#else
#  define _GPIO_MASK       (0x3FC)
#  define _GPIO_SHIFT_BITS (2)
#  define _OE1_PIN         (10)
#  define _OE2_PIN         (11)
#  define _OE3_PIN         (20)
#  define _RW_PIN          (21)
#  define _CLOCK_PIN       (22)
#  define _AUDIO_PIN       (27)
#  define _RESET_PIN       (26)
#  define _IRQ_PIN         (28)
#endif  // PICO_NEO6502

void wdc65C02cpu_init() {
    gpio_init_mask(_GPIO_MASK);

    gpio_init(_OE1_PIN);  // OE1
    gpio_set_dir(_OE1_PIN, GPIO_OUT);
    gpio_put(_OE1_PIN, 1);

    gpio_init(_OE2_PIN);  // OE2
    gpio_set_dir(_OE2_PIN, GPIO_OUT);
    gpio_put(_OE2_PIN, 1);

    gpio_init(_OE3_PIN);  // OE3
    gpio_set_dir(_OE3_PIN, GPIO_OUT);
    gpio_put(_OE3_PIN, 1);

    gpio_init(_RW_PIN);  // RW
    gpio_set_dir(_RW_PIN, GPIO_IN);

    gpio_init(_CLOCK_PIN);  // CLOCK
    gpio_set_dir(_CLOCK_PIN, GPIO_OUT);

    gpio_init(_RESET_PIN);  // RESET
    gpio_set_dir(_RESET_PIN, GPIO_OUT);

    gpio_init(_IRQ_PIN);  // IRQ
    gpio_set_dir(_IRQ_PIN, GPIO_OUT);
    gpio_put(_IRQ_PIN, 1);

#ifdef PICO_NEO6502
    gpio_init(_NMI_PIN);  // NMI
    gpio_set_dir(_NMI_PIN, GPIO_OUT);
    gpio_put(_NMI_PIN, 1);
#endif  // PICO_NEO6502

    gpio_put(_RESET_PIN, 0);
    sleep_us(1000);
    gpio_put(_RESET_PIN, 1);
}

void wdc65C02cpu_reset() {
    gpio_put(_RESET_PIN, 0);
    sleep_us(1000);
    gpio_put(_RESET_PIN, 1);
}

void wdc65C02cpu_nmi() {
#ifdef PICO_NEO6502
    gpio_put(_NMI_PIN, 0);
    sleep_us(1000);
    gpio_put(_NMI_PIN, 1);
#endif  // PICO_NEO6502
}

void __not_in_flash_func(wdc65C02cpu_tick)(uint16_t* addr, bool* rw) {
    gpio_put(_CLOCK_PIN, 0);

    *addr = wdc65C02cpu_get_address();

    *rw = gpio_get(_RW_PIN);

    gpio_put(_CLOCK_PIN, 1);
}

uint16_t wdc65C02cpu_get_address() {
    gpio_set_dir_masked(_GPIO_MASK, 0);

    gpio_put(_OE1_PIN, 0);
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#ifndef PICO_NEO6502
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#endif
    uint16_t addr = (gpio_get_all() >> _GPIO_SHIFT_BITS) & 0xFF;
    gpio_put(_OE1_PIN, 1);

    gpio_put(_OE2_PIN, 0);
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#ifndef PICO_NEO6502
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#endif
    addr |= (gpio_get_all() << (8 - _GPIO_SHIFT_BITS)) & 0xFF00;
    gpio_put(_OE2_PIN, 1);

    // printf("get addr: %04x\n", addr);

    return addr;
}

uint8_t __not_in_flash_func(wdc65C02cpu_get_data)() {
    gpio_set_dir_masked(_GPIO_MASK, 0);

    gpio_put(_OE3_PIN, 0);
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#ifndef PICO_NEO6502
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#endif
    uint8_t data = (gpio_get_all() >> _GPIO_SHIFT_BITS) & 0xFF;
    gpio_put(_OE3_PIN, 1);

    // printf("get data: %02x\n", data);

    return data;
}

void wdc65C02cpu_set_data(uint8_t data) {
    gpio_set_dir_masked(_GPIO_MASK, _GPIO_MASK);

    gpio_put_masked(_GPIO_MASK, data << _GPIO_SHIFT_BITS);
    gpio_put(_OE3_PIN, 0);
#ifndef PICO_NEO6502
    __asm volatile("nop\n");
    __asm volatile("nop\n");
#endif
    gpio_put(_OE3_PIN, 1);

    // printf("set data: %02x\n", data);
}

void wdc65C02cpu_set_irq(bool state) {
    gpio_put(_IRQ_PIN, state ? 0 : 1);
}

bool rw;
uint ticks6502;

#define TICK_CALLS 40

void __not_in_flash_func(_tick6502)(void){
    ticks6502++;
	wdc65C02cpu_tick(&address, &rw);                                           // Tick the processor
	if (rw) {                
        //uint8_t data = mem[address];                                               // Read put data on data lines.
        uint8_t data = memory_read_data();
		wdc65C02cpu_set_data(data);
	} else {                                                                // Write get it and store in memory.
		data = wdc65C02cpu_get_data();
        memory_write_data(data);
	}  	
}

void __not_in_flash_func(tick6502)(void){
    for (int i=0;i<TICK_CALLS;i++){
        _tick6502();
        __asm volatile("nop\n");
        __asm volatile("nop\n");        
    }
}



#endif /* CHIPS_IMPL */
