#include "ng_utils.h"

#include <pico/time.h>
#include <malloc.h>

extern void* assets[];

uint32_t utils_millis(void) {
    return to_ms_since_boot(get_absolute_time());
}

uint32_t utils_get_heap_total(void) {
   extern char __StackLimit, __bss_end__;
   
   return &__StackLimit  - &__bss_end__;
}

uint32_t utils_get_heap_free(void) {
   struct mallinfo m = mallinfo();

   return utils_get_heap_total() - m.uordblks;
}

// get void pointer for an asset_id
void* asset_get_pointer(uint8_t asset_id) {
   // TODO: some sort of check if at least this asset-id exists...
   return assets[asset_id];
}

void utils_print_binary(const char* prefix,int n) {
    printf("%s", prefix);
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}