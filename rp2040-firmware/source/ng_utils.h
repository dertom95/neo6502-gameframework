#ifndef __NG_UTILS_H__
#define __NG_UTILS_H__

#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// return the ms since application start
uint32_t utils_millis(void);
uint32_t utils_get_heap_total(void);
uint32_t utils_get_heap_free(void);

void    utils_print_binary(const char* prefix,int n) ;

#endif