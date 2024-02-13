#ifndef __NG_DEFINES_H_
#define __NG_DEFINES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef PICO_NEO6502
# include "pico/platform.h"
#else
  #define __not_in_flash_func(FUNC) FUNC
#endif

#define SEGMENT_GFX_DATA 0
#define SEGMENT_6502_MEM 1

#define ASSET_TYPE_TILESHEET 1
#define ASSET_TYPE_PALETTE   2

#endif