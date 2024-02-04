#ifndef __NG_UTILS_H__
#define __NG_UTILS_H__

#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// return the ms since application start
uint32_t utils_millis(void);
uint32_t utils_get_heap_total(void);
uint32_t utils_get_heap_free(void);

// get void pointer for an asset_id
void*   asset_get_pointer(uint8_t asset_id);
void    utils_print_binary(const char* prefix,int n) ;

#endif