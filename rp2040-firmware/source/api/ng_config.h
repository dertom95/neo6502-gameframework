#ifndef __NG_CONFIG_H_
#define __NG_CONFIG_H_

#include "../ng_defines.h"
#include "ng_api_shared.h"

#define GAMEPAD_MAX_DEVICES 4

#define SCREEN_WIDTH 320
#define SCREEN_WIDTH_HALF 160
#define SCREEN_HEIGHT 240

#define CPU6502_MEMORY_SIZE  0x10000 // 64k
#define GFX_MEMORY_SIZE 100000
//#define GFX_MEMORY_SIZE 320*240

#define GFX_RENDERQUEUE_MAX_ELEMENTS 5

#define MEMORY_MAP_END 0xfff9

// sizes should be uneven so that the next address will start on an even number
// otherwise ARM might not like that and crash. ARM don't like to use unaligned addresses ;)
#define MEMORY_MAP_FUNC_END   (MEMORY_MAP_END)
#define MEMORY_MAP_FUNC_START (MEMORY_MAP_FUNC_END-79)

#define MEMORY_MAP_CALL_BUFFER_END   (MEMORY_MAP_FUNC_START - 1)
#define MEMORY_MAP_CALL_BUFFER_BEGIN (MEMORY_MAP_CALL_BUFFER_END - 31)

#define MEMORY_MAP_CALLRETURN_BUFFER_END (MEMORY_MAP_CALL_BUFFER_BEGIN - 1)
#define MEMORY_MAP_CALLRETURN_BUFFER_BEGIN (MEMORY_MAP_CALLRETURN_BUFFER_END - 13)

#define MEMORY_MAP_VARMAPS_END (MEMORY_MAP_CALLRETURN_BUFFER_BEGIN - 1)

#define FUNCTION_RETURN_OK 0x01
#define FUNCTION_RETURN_ERROR 0x02

#define MM_FUNCS (MEMORY_MAP_FUNC_START) /* memory-location where the memory-mapping starts */
#define MM_FUNC_CALL (MM_FUNCS + 0x00)   /* uint8 */

#endif