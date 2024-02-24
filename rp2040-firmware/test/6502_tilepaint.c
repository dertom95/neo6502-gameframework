
#include<stdint.h>
#include "../source/api/ng_api.h"


int main(){
    uint8_t* counter = (uint8_t*)(0x2000);
    *counter = 0;
    while(1){
        *counter=(*counter)+1;
    }
    return 0;
}

// int main(){
//     uint16_t i=MEMORY_TILES_TOTALSPACE;
//     uint8_t* tilemap;
//     uint8_t col=0;
//     uint8_t iters = 3;

//     tilemap = (uint8_t*)MEMORY_TILEAREA_BEGIN;
//     while(1){
//         *(tilemap++)=col++;        
//         i--;
//         if (i==0){
//             tilemap = (uint8_t*)MEMORY_TILEAREA_BEGIN;
//             i=MEMORY_TILES_TOTALSPACE;
//             if (iters--==0){
//                 break;
//             }
//         }
//     }
//     {
//         int a=0;
//         tilemap = (uint8_t*)MEMORY_TILEAREA_BEGIN+90;
//         while (1){
//             while(a!=0){
//                 a--;
//             }
//             a=255;
//             *(tilemap)=col++;
//         }
//     }
//     return 0;
// }

// int main(){
//     uint8_t* tilegfx;
//     volatile uint8_t* key_pressed;

//     key_pressed = (uint8_t*)MEMORY_MAP_FUNC_START;
//     tilegfx = (uint8_t*)MM_KEYSET;
    
//     {
//         int8_t posx,posy,col;
//         posx=0;
//         posy=0;
//         col=5;
//         while(1){
//             switch(*key_pressed){
//                 case 'a' : posx--; break;
//                 case 'd' : posx++; break;
//                 case 'w' : posy--; break;
//                 case 's' : posx++; break;
//             }

//             if (posx<0) posx=0;
//             if (posx>=MEMORY_TILES_WIDTH) posx=MEMORY_TILES_WIDTH-1;
//             if (posy<0) posy=0;
//             if (posy>=MEMORY_TILES_HEIGHT) posx=MEMORY_TILES_HEIGHT-1;

//             *(tilegfx+posx+posy*MEMORY_TILES_WIDTH)=col;
//         }
//     }
    
//     return 0;
// }