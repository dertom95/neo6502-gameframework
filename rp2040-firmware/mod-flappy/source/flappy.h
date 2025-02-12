#ifndef __FLAPPY__
#define __FLAPPY__

#include <stdint.h>

#define GS_READY 1
#define GS_ALIVE 2
#define GAMEOVER 3
#define GAMEOVER_HIGHSCORE 4

#define AUDIO_CRASH     0
#define AUDIO_POWERUP   1
#define AUDIO_FLY       2

#define W 320
#define H 240
//#define GROUND 80
#define GROUND 0
#define PIPE_W 48
#define PHYS_W (W + PIPE_W + 80)
#define GAP 80
#define GRACE 4
#define PLYR_X 80
#define PLYR_SZ 32

typedef struct gamedata_t {
    uint8_t gamestate;
    float player_y;
    float player_vel;
    int pipe_x[2];
    float pipe_y[2];
    int score;
    int best;
    int idle_time;
    float frame;
} gamedata_t;


void flappy_init(void);
void flappy_tick(void);
void flappy_on_actionbutton(void);
void flappy_play_audio(uint8_t audio_id);
gamedata_t* get_gamedata();

#endif