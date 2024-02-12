#include "ng_utils.h"

#include<stdio.h>

extern void* assets[];

void utils_print_binary(const char* prefix,int n) {
    printf("%s", prefix);
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}