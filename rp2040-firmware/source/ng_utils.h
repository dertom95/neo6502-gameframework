#ifndef __NG_UTILS_H__
#define __NG_UTILS_H__

#include <stdint.h>

// return the ms since application start
uint32_t utils_millis(void);
uint32_t utils_get_heap_total(void);
uint32_t utils_get_heap_free(void);

// get void pointer for an asset_id
void*    asset_get_pointer(uint8_t asset_id);


#define flags_set(FLAGS,MASK) FLAGS |= MASK;
#define flags_unset(FLAGS,MASK) FLAGS &= ~MASK;
#define flags_isset(FLAGS,MASK) ((FLAGS & MASK)==MASK)
#define flags_isset_some(FLAGS,MASK) ((FLAGS & MASK)>0)

#endif