#ifndef __NG_DEFINES_H_
#define __NG_DEFINES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// userspace address (6502)=>16bit
typedef uint8_t* US_ADDRESS;

#define ASSERT_MODE_STRICT
#define LITTLEENDIAN

#ifdef ASSERT_MODE_STRICT
# define ASSERT_STRICT(X) assert(X)
#else
# define ASSERT_STRICT(X) void
#endif

#ifdef LITTLEENDIAN
// no need to swap
# define swap16(val) (val)
#else
# define swap16(val) (val << 8) | (val >> 8)
#endif


#ifdef PICO_NEO6502
# include "pico/platform.h"
#else
# define __not_in_flash_func(FUNC) FUNC
#endif

#define SEGMENT_GFX_DATA 0
#define SEGMENT_6502_MEM 1

#define ASSET_TYPE_TILESHEET 1
#define ASSET_TYPE_PALETTE   2




#if PICO_NEO6502
# include "core/backend/neo6502/neo6502_defines.h"
#elif __KINC__
# include "core/backend/kinc/kinc_defines.h"
#endif

#endif
