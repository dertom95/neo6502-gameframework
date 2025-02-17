#include "ng_utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#include "ng_defines.h"

void utils_print_binary(const char* prefix,int n) {
    printf("%s", prefix);
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

typedef struct idstore_t {
    uint8_t pointer_amount;
    void** pointers;
    uint8_t recycled_amount;
    uint8_t* recycled;
    uint8_t max_ids;
} idstore_t;

idstore_t idstore={0};

volatile int b = 0;

void ng_debug_value(uint16_t v1, uint16_t v2){
    int a=0;
    b = v1;
    //printf("a");
}

void ng_debug_pointer(void* ptr, uint8_t data){
    int a=0;
    data = 0;
    printf("a");
}

void id_init(uint8_t max_ids){
    assert(max_ids<255 && "max-ids must be [1..254]");
    idstore.pointer_amount=1; // start with idx 1. id==0 is considered not set
    idstore.max_ids  = max_ids;
    idstore.pointers = malloc(sizeof(void*)*max_ids);
    idstore.recycled = malloc(sizeof(uint8_t)*max_ids);
}

uint8_t id_store(void* ptr) {
    assert(ptr!=NULL && "cannot store NULL-PTR");
    uint8_t idx;
    if (idstore.recycled_amount>0){
        // use a recycled id
        idx = idstore.recycled[idstore.recycled_amount-1];
        assert(idstore.pointers[idx]==NULL && "recycled pointer is not NULL?!");
        idstore.recycled[idstore.recycled_amount-1]=0;
        idstore.recycled_amount--;
    } else {
        assert(idstore.pointer_amount<idstore.max_ids && "EXCEEDED ID-Max");
        idx = idstore.pointer_amount++;
    }
    idstore.pointers[idx]=ptr;
    return idx;
}

void id_release(uint8_t id) {
    idstore.recycled[idstore.recycled_amount++]=id;
    idstore.pointers[id]=NULL;
}

void* id_get_ptr(uint8_t id){
    assert(id < idstore.pointer_amount && "idstore: idx out of bounds");
    assert(idstore.pointers[id]!=NULL && "id-store prt is not set!");
    return idstore.pointers[id];
}

void id_assert_validity(uint8_t id){
    assert(id < idstore.pointer_amount && "idstore: idx out of bounds");
    assert(idstore.pointers[id]!=NULL && "id-store prt is not set!");    
}

void ll_add(linked_list_t** start, linked_list_t* element) {
    assert(start != NULL);

    // If the list is empty, make the new element as the start
    if (*start == NULL) {
        *start = element;
        element->next = NULL;
        return;
    }

    // Traverse to the end of the list
    linked_list_t* current = *start;
    while (current->next != NULL) {
        current = current->next;
    }

    // Append the new element to the end of the list
    current->next = element;
    element->next = NULL;
}

void ll_remove(linked_list_t** start, linked_list_t* element) {
    assert(start != NULL);
    assert(*start != NULL);

    // If the element to be removed is at the start
    if (*start == element) {
        *start = element->next;
        element->next = NULL;
        return;
    }

    // Traverse the list to find the element to be removed
    linked_list_t* current = *start;
    linked_list_t* prev = NULL;
    while (current != NULL && current != element) {
        prev = current;
        current = current->next;
    }

    // If the element is found, remove it from the list
    if (current != NULL) {
        prev->next = current->next;
        current->next = NULL;
    }
}


static uint16_t lfsr = 0xABCD; // Initial seed (can be any non-zero value)
uint16_t utils_random_uint16() {
    // Perform the LFSR operation
    uint16_t lsb = lfsr & 1; // Get the least significant bit
    lfsr >>= 1;             // Shift right
    if (lsb) {
        lfsr ^= 0xB400;     // XOR with a 16-bit polynomial (e.g., 0xB400)
    }
    return lfsr;          
}

char binary_str[9];
const char* utils_char_to_binstring(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        binary_str[7 - i] = (value & (1 << i)) ? '1' : '0';
    }
    binary_str[8] = '\0'; // Null-terminate the string
    return binary_str;
}