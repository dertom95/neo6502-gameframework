#ifndef ASSETS_H
#define ASSETS_H
#include<stdint.h>

#define ASSET_CODE 0
#define ASSET_COLOR_PALETTE 1
#define ASSET_COLOR_PALETTE_SMALL 2
#define ASSET_FONT8 3
#define ASSET_OLD_GUY 4
#define ASSET_SPRITES_MISC 5
#define ASSET_SPRITESHEET 6
#define ASSET_TILEMAP 7

extern const uint32_t assets_offsets[];
extern const uint32_t assets_sizes[];

#define ASSETS_AMOUNT 8

#ifdef INCLUDE_DATA
extern const uint8_t assets_data[];
#endif
#endif // ASSETS_H
