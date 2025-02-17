#ifndef __NG_UTILS_H__
#define __NG_UTILS_H__

#include <stdint.h>
#include <assert.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clamp(val, low, high) (max((low), min((val), (high))))

#define bit_is_set_all(FLAG,MASK) (((FLAG)&(MASK))==(MASK))
#define bit_is_set_some(FLAG,MASK) (((FLAG)&(MASK))>0)
#define bit_set(FLAG,MASK) (FLAG)|=(MASK)
#define bit_unset(FLAG,MASK) (FLAG)&=~(MASK)

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
void     id_assert_validity(uint8_t id);

void ll_add(linked_list_t** start, linked_list_t* element);
void ll_remove(linked_list_t** start, linked_list_t* element);

// convert char to string. returning temp_char (that will be overwritten on later calls)
const char* utils_char_to_binstring(uint8_t ch);
/*api:6:2*/uint16_t utils_random_uint16();
// debug values from the 6502 in the backend
/*api:6:1*/void     ng_debug_value(uint16_t v1,uint16_t v2);
/*api:6:3*/void ng_debug_pointer(void* ptr, uint8_t data);

// PLEASE: ALWAYS MAINTAIN: LAST API ID 6:3
#endif
