#ifndef __NG_UTILS_H__
#define __NG_UTILS_H__

#include <stdint.h>

// return the ms since application start
uint32_t utils_millis(void);
uint32_t utils_get_heap_total(void);
uint32_t utils_get_heap_free(void);

#endif