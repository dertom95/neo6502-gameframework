#ifndef ASSETS_H
#define ASSETS_H
#include<stdint.h>

#define ASSET_COLOR_PALETTE 0
#define ASSET_COLOR_PALETTE_SMALL 1
#define ASSET_FONT8 2
#define ASSET_OLD_GUY 3
#define ASSET_SPRITES_MISC 4

extern const uint32_t assets_offsets[];
extern const uint32_t assets_sizes[];

#define ASSETS_AMOUNT 5

#ifdef INCLUDE_DATA
extern const uint8_t assets_data[];
#endif
#endif // ASSETS_H
