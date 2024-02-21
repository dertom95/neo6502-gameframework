// // roms.h

#ifndef _ROMS_h
#define _ROMS_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool loadROMS();
bool loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize);

#ifdef __cplusplus
}
#endif

#endif


