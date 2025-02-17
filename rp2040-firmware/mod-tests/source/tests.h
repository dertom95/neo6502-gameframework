#ifndef __TESTS_H_
#define __TESTS_H_

#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include "../../source/ng_all.h"
#endif

#include "export/assets.h"
#include <ng_api_shared.h>

extern gfx_pixelbuffer_t pxb_instructions;

void test1_init();
void test1_update();
void test1_clear();
void draw_instructions(const char* instructions);

#endif __TESTS_H