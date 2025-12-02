#ifndef __moorhuhn_h_
#define __moorhuhn_h_

#include <stdint.h>
#include <stdbool.h>

#include "mh_globals.h"

#define MH_HUHNFLAG_ALIVE (1 << 0)

#define MH_HUHNTYPE_DEFAULT 0

typedef struct mh_huhn_t {
    int16_t x,y;
    int8_t velocity;
    uint8_t flags;

    uint8_t hitpoints;
    uint8_t sprite_id;
} mh_huhn_t;

typedef struct mh_gamestate_t {
    uint8_t flags;
    uint8_t mhs_amount;
    mh_huhn_t mhs[MAX_MOORHUHN_AMOUNT];
} mh_gamestate_t;

extern mh_gamestate_t mh_gs;

void mh_init();
void mh_tick();

void mh_render();

void mh_update_huhn_positions();
bool mh_shoot_at(int16_t x,int16_t y);


bool mh_huhn_find_free(uint8_t* huhn_idx, mh_huhn_t** huhn);
void mh_huhn_remove_at_idx(uint8_t idx);
bool mh_huhn_remove(mh_huhn_t* huhn);
bool mh_huhn_spawn(mh_huhn_t* huhn_data, uint8_t type, mh_huhn_t** out_huhn);

#endif