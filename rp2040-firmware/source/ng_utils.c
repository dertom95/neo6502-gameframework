#include "ng_utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

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

void     id_release(uint8_t id) {
    idstore.recycled[idstore.recycled_amount++]=id;
    idstore.pointers[id]=NULL;
}

void* id_get_ptr(uint8_t id){
    ASSERT_STRICT(idstore.pointers[id]!=NULL && "id-store prt is not set!");
    return idstore.pointers[id];
}