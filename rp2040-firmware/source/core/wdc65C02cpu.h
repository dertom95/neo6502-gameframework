#ifndef __WDC65C02CPU_H_
#define __WDC65C02CPU_H_

/*
    wdc65C02cpu.h    --

    TODO: docs

    ## zlib/libpng license

    Copyright (c) 2018 Andre Weissflog
    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.
        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.
        3. This notice may not be removed or altered from any source
        distribution.
*/
#include <stdint.h>
#include <stdbool.h>
#include "memory.h"

// initialize cpu
void wdc65C02cpu_init();
// reset cpu
void wdc65C02cpu_reset();

void wdc65C02cpu_nmi();

// tick the cpu
void wdc65C02cpu_tick(uint16_t* addr, bool* rw);

uint16_t wdc65C02cpu_get_address();

uint8_t wdc65C02cpu_get_data();

void wdc65C02cpu_set_data(uint8_t data);

void wdc65C02cpu_set_irq(bool state);

void tick6502();

#endif