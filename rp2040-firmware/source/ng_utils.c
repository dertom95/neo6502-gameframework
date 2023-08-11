#include "ng_utils.h"

#include <pico/time.h>
#include <malloc.h>

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