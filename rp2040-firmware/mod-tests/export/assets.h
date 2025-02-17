#ifndef ASSETS_H
#define ASSETS_H
#include<stdint.h>

#define ASSET_CODE 0
#define ASSET_COLOR_PALETTE 1
#define ASSET_FONT8 2
#define ASSET_UI 3
#define ASSETS_AMOUNT 4

#ifdef INCLUDE_DATA
extern const uint8_t assets_data[];
#endif
#endif // ASSETS_H
