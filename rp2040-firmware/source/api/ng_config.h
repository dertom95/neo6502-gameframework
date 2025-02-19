#ifndef __NG_CONFIG_H_
#define __NG_CONFIG_H_

#include "../ng_defines.h"
#include "ng_api_shared.h"
#include "ng_memory_map.h"

#define GAMEPAD_MAX_DEVICES 4

#define SCREEN_WIDTH 320
#define SCREEN_WIDTH_HALF 160
#define SCREEN_HEIGHT 240

#define CPU6502_MEMORY_SIZE  0x10000 // 64k 
#define GFX_MEMORY_SIZE 100000
//#define GFX_MEMORY_SIZE 320*240

#define GFX_RENDERQUEUE_MAX_ELEMENTS 5



#endif