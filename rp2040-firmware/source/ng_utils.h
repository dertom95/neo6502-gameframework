#ifndef __NG_UTILS_H__
#define __NG_UTILS_H__

#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct linked_list_t {
    struct linked_list_t* next;
    void* data;
} linked_list_t;

// return the ms since application start
uint32_t utils_millis(void);
uint32_t utils_get_heap_total(void);
uint32_t utils_get_heap_free(void);

void     utils_print_binary(const char* prefix,int n) ;

void     id_init(uint8_t max_ids);
uint8_t  id_store(void* ptr);
void*    id_get_ptr(uint8_t id);
void     id_release(uint8_t id);

void ll_add(linked_list_t** start, linked_list_t* element);
void ll_remove(linked_list_t** start, linked_list_t* element);
#endif