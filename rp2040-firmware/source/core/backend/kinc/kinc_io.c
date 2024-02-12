#ifdef __KINC__

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 No0ne (https://github.com/No0ne)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "kinc_ng.h"

#include "../../../ng_io.h"
#include "../../../ng_gfx.h"
#include "../../memory.h"

#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>

#define KEYBOARD_REPORT_MAX_KEYCODES 6

typedef struct hid_keyboard_report_t
{
  uint8_t modifier;   
  uint8_t keycode_amount;
  uint8_t keycode[KEYBOARD_REPORT_MAX_KEYCODES]; 
} hid_keyboard_report_t;

hid_keyboard_report_t kb_pressed;
hid_keyboard_report_t kb_down;
hid_keyboard_report_t kb_released;

static void keyboard_cb(int key, void* data)
{
  hid_keyboard_report_t* report = data;
  report->keycode[report->keycode_amount++]=key;
}

void io_backend_init()
{
  kinc_keyboard_set_key_press_callback(&keyboard_cb, &kb_pressed);
  kinc_keyboard_set_key_down_callback(&keyboard_cb, &kb_down);
  kinc_keyboard_set_key_up_callback(&keyboard_cb, &kb_released);
}

void io_backend_update()
{
  kb_pressed=(hid_keyboard_report_t){0};
  kb_down=(hid_keyboard_report_t){0};
  kb_released=(hid_keyboard_report_t){0};
}

// look up new key in previous keys
bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<KEYBOARD_REPORT_MAX_KEYCODES; i++)
  {
    if (report->keycode[i] == keycode)  {
      return true;
    }
  }

  return false;
}

bool io_keyboard_is_pressed(uint8_t keycode){
  bool result = find_key_in_report(&kb_pressed,keycode);
  return result;
}

bool io_keyboard_is_down(uint8_t keycode){
  return find_key_in_report(&kb_down,keycode);
}

bool io_keyboard_is_released(uint8_t keycode){
  return find_key_in_report(&kb_released,keycode);
}

#endif