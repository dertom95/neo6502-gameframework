#ifndef ASSETS_H
#define ASSETS_H
#include<stdint.h>

#define ASSET_CODE 0
#define ASSET_BITMAP_BACKGROUND 1
#define ASSET_COLOR_PALETTE 2
#define ASSET_FONT8 3
#define ASSET_SPRITE_BIRD 4
#define ASSET_SPRITE_PILLAR 5
#define ASSET_POWER_UP_8 6
#define ASSET_GAME 7
#define ASSETS_AMOUNT 8

#ifdef INCLUDE_DATA
extern const uint8_t assets_data[];
#endif
#endif // ASSETS_H
