#include "flappy.h"

#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
# define DEBUG_PRINT(fmt, ...) \
    printf("DEBUG: %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#else
# include <ng_config.h>
# include <gen/ng_api.h>
# define DEBUG_PRINT(fmt, ...) 
#endif



// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// int seed = 13;

// int random() {
//     seed = seed * 1103515245 + 12345;
//     return (unsigned int)(seed / 65536) % 32768;
// }

static uint8_t lfsr = 0xAB; // Example seed value

// Function to generate a random number
uint8_t generate_random_lfsr() {
    // Perform the LFSR operation
    uint8_t lsb = lfsr & 1; // Get the least significant bit
    lfsr >>= 1;             // Shift right
    if (lsb) {
        lfsr ^= 0xB8;       // XOR with a polynomial (e.g., 0xB8 for 8-bit LFSR)
    }
    return lfsr;            // Return the new random number
}

#define RANDOM_PIPE_HEIGHT (generate_random_lfsr() % (H - GROUND - GAP - 60) + 30)


gamedata_t gd = {
    .player_y = (H - GROUND) >> 2,
    .pipe_x = {W,W},
    .pipe_y = {0,0},
    .idle_time = 30,
    .gamestate = GS_READY,
};

void setup();
void new_game();
void update_stuff();
void update_pipe(int i);
void draw_stuff();
void text(char *fstr, int value, int height);

gamedata_t* get_gamedata() {
    return &gd;
}

void flappy_init(void){

}

void flappy_on_actionbutton(void) {
    ng_debug_value(01,gd.gamestate);
    if(gd.gamestate == GS_ALIVE)
    {
//TODO:            gd.player_vel = -11.7f; 
        gd.player_vel = -5.7f;
        gd.frame += 1.0f;
        flappy_play_audio(AUDIO_FLY);
    }
    else if(gd.idle_time > 30)
    {
            new_game();
    }    
}

void update_stuff(void);
extern void draw_stuff(void);

void flappy_tick(void){
    io_before_tick();
    update_stuff();
    draw_stuff();
    // TODO:
    // SDL_Delay(1000 / 60);
    gd.idle_time++;    
}

void new_game()
{
    gd.gamestate = GS_ALIVE;
    gd.player_y = (H - GROUND)/2;
    // gd.player_vel = -11.7f; // TODO
    gd.player_vel = -0.7f;
    gd.score = 0;
    gd.pipe_x[0] = PHYS_W + PHYS_W/2 - PIPE_W;
    gd.pipe_x[1] = PHYS_W - PIPE_W;
    gd.pipe_y[0] = RANDOM_PIPE_HEIGHT;
    gd.pipe_y[1] = RANDOM_PIPE_HEIGHT;
}

//when we hit something
void game_over()
{
    flappy_play_audio(AUDIO_CRASH);
    gd.gamestate = GAMEOVER;
    gd.idle_time = 0;
    if(gd.best < gd.score) {
        gd.best = gd.score;
        gd.gamestate = GAMEOVER_HIGHSCORE;
    }
}

//update everything that needs to update on its own, without input
void update_stuff()
{
        if(gd.gamestate != GS_ALIVE) return;

        gd.player_y +=gd.player_vel;
        //DEBUG_PRINT("Player Y:%d\n",gd.player_y);
        
        gd.player_vel += 0.61; // gravity

        if(gd.player_vel > 10.0f){
            gd.frame = 0;
        } else {
            gd.frame -= (gd.player_vel - 10.0f) * 0.03f; //fancy animation
        }

        if(gd.player_y > H - GROUND - PLYR_SZ){
            game_over();
        }

        for(int i = 0; i < 2; i++) {
            update_pipe(i);
        }
}

const int player_x = PLYR_X + PLYR_SZ;

//update one pipe for one frame
void update_pipe(int i)
{
        bool player_x_check = player_x >= gd.pipe_x[i] + GRACE && PLYR_X <= gd.pipe_x[i] + PIPE_W - GRACE;
        bool player_y_check1 = gd.player_y <= gd.pipe_y[i] - GRACE;
        bool player_y_check2 = gd.player_y + PLYR_SZ >= gd.pipe_y[i] + GAP + GRACE;
        if( player_x_check &&
            (player_y_check1 || player_y_check2 )) {

            game_over(); // player hit pipe
        }

        // move pipes and increment score if we just passed one
        gd.pipe_x[i] -= 5;
        if(gd.pipe_x[i] + PIPE_W < PLYR_X && gd.pipe_x[i] + PIPE_W > PLYR_X - 5){
            flappy_play_audio(AUDIO_POWERUP);
            gd.score++;
        }

        // respawn pipe once far enough off screen
        if(gd.pipe_x[i] <= -PIPE_W)
        {
                gd.pipe_x[i] = PHYS_W - PIPE_W;
                gd.pipe_y[i] = RANDOM_PIPE_HEIGHT;
        }
}



