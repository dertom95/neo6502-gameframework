#ifndef ASSETS_H
#define ASSETS_H
#include<stdint.h>

#define ASSET_CODE 0
#define ASSET_BITMAP_BG1 1
#define ASSET_COLOR_PALETTE 2
#define ASSET_FONT8 3
#define ASSET_SPRITE_BIRD_16 4
#define ASSET_SPRITE_BIRD_32 5
#define ASSET_SPRITE_CROSSHAIR_16 6
#define ASSET_CRASH 7
#define ASSET_POWER_UP_8 8
#define ASSET_GAME 9
#define ASSETS_AMOUNT 10

#ifdef INCLUDE_DATA
extern const uint8_t assets_data[];
#endif
#endif // ASSETS_H
