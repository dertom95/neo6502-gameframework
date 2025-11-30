#ifndef __NG_DEFINES_H_
#define __NG_DEFINES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// userspace address (6502)=>16bit
typedef uint8_t* US_ADDRESS;

#ifndef __NG6502__
# define ASSERT_MODE_STRICT
#endif

#define LITTLEENDIAN

#ifdef ASSERT_MODE_STRICT
# define ASSERT_STRICT(X) assert(X)
#else
# define ASSERT_STRICT(X) (void)(0)
#endif

#ifdef LITTLEENDIAN
// no need to swap
# define swap16(val) (val)
#else
# define swap16(val) (val << 8) | (val >> 8)
#endif

#define __align4 __attribute__((aligned(4)))

#ifdef PICO_NEO6502
#include "pico/stdlib.h"
# include "boards/pico.h"
#else
# define __not_in_flash_func(FUNC) FUNC
#endif

#define SEGMENT_GFX_DATA 0
#define SEGMENT_6502_MEM 1

#define ASSET_TYPE_MASK (3 << 0)
#define ASSET_TYPE_TILESHEET (0 << 0)
#define ASSET_TYPE_PALETTE   (1 << 0)

#define ASSET_TYPE_FILEFORMAT_MASK (3 << 2)
#define ASSET_TYPE_FILEFORMAT_1    (0 << 2)
#define ASSET_TYPE_FILEFORMAT_2    (1 << 2)

//#define DEBUG_TRANSPARENCY 





#if PICO_NEO6502
# include "core/backend/neo6502/neo6502_defines.h"
#elif __KINC__
# include "core/backend/kinc/kinc_defines.h"
#endif

#endif
