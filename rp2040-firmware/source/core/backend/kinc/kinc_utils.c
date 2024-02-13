#include "../../../ng_utils.h"
#include "../../../ng_gfx.h"
#include "../../../ng_mem.h"
#include <malloc.h>
#include <kinc/system.h>

extern void* assets[];

uint32_t utils_millis(void) {
   double time = kinc_time();
   uint32_t result = (uint32_t)(time * 1000);
   return result;
}

uint32_t utils_get_heap_total(void) {
   return 0;
   // extern char __StackLimit, __bss_end__;
   
   // return &__StackLimit  - &__bss_end__;
}

uint32_t utils_get_heap_free(void) {
   return 0;
   // struct mallinfo m = mallinfo();

   // return utils_get_heap_total() - m.uordblks;
}

// get void pointer for an asset_id
void* asset_get_pointer(uint8_t asset_id) {
   // TODO: some sort of check if at least this asset-id exists...
   return assets[asset_id];
}



