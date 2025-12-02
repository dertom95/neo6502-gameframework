#include "mh_globals.h"

gfx_sprite_animator_t anim4x1=
    {
        .animation_amount=2,
        .animations={
            {
                .start_tile=0,
                .end_tile=3,
                .delay_ms=120,
            }
            ,{
                .start_tile=4,
                .end_tile=7,
                .delay_ms=60,
            }
        }
};

gfx_sprite_animator_t anim4x1_60=
    {
        .animation_amount=1,
        .animations={
            {
                .start_tile=0,
                .end_tile=3,
                .delay_ms=60,
            }
        }
};