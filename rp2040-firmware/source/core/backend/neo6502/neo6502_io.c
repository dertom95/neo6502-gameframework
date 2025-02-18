#ifdef PICO_NEO6502

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

#include "neo6502.h"

#include <signal.h>
#define __BREAKPOINT__ raise(SIGINT);

#include "../../../ng_io.h"
#include "../../../ng_gfx.h"
#include "../../memory.h"
#include "../../../ng_utils.h"

#include "class/hid/hid.h"

#define DEBUG 1

#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"

#if DEBUG
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 28
#endif

#include <ctype.h>
#include "ff.h"
#include "diskio.h"

uint8_t kbd_addr = 0;
uint8_t kbd_inst = 0;

bool finished_usb_startup = false;

uint8_t const hid2ascii[] = {
  0x00, 	//0-3 empty
  0x00, 
  0x00, 
  0x00, 
  'a', 		//a..z 1..0
  'b',
  'c',
  'd',
  'e',
  'f',
  'g',
  'h',
  'i',
  'j',
  'k',
  'l',
  'm',
  'n',
  'o',
  'p',
  'q',
  'r',
  's',
  't',
  'u',
  'v',
  'w',
  'x',
  'y',
  'z',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  '0',
  0x0D, 	//enter
  0x1B,		//esc
  0x08,		//BS
  0x09,		//TAB
  ' ',
  '-',
  '=',
  '[',
  ']',
  0x5C,		// backslash
  0x00,
  ';',
  0x27,		//single quote
  '`',
  ',',
  '.',
  '/',
  0x00,		//CAPS
  0x00,		//F1
  0x00,		//F2
  0x00,		//F3
  0x00,		//F4
  0x00,		//F5
  0x00,		//F6
  0x00,		//F7
  0x00,		//F8
  0x00,		//F9
  0x00,		//F10
  0x00,		//F11
  0x00,		//F12
  0x00,		//PrtScr
  0x00,		//ScrlLock
  0x00,		//Pause/Break
  0x00,		//Ins
  0x00,		//Home
  0x00,		//PgUp
  0x00,		//Del
  0x00,		//End
  0x00,		//PgDn
  0x00,		//Right-Arr
  0x00,		//Left-Arr
  0x00,		//Down-Arr
  0x00,		//Up-Arr
  0x00,		//NUMPAD ----NumLock
  0x00,		// /
  0x00,		// *
  0x00,		// -
  0x00,		// +
  0x00,		// ???
  0x00,		// 1
  0x00,		// 2
  0x00,		// 3
  0x00,		// 4
  0x00,		// 5
  0x00,		// 6
  0x00,		// 7
  0x00,		// 8
  0x00,		// 9	
  0x00,		// Ins
  0x00,		// Del
  '<'
};

uint8_t const hid2asciiCtrl[] = {
  0x00, 	//0-3 empty
  0x00, 
  0x00, 
  0x00, 
  0x01, 	//^A
  0x02,
  0x03,
  0x04,
  0x05,
  0x06,
  0x07,
  0x08,
  0x09,
  0x0A,
  0x0B,
  0x0C,
  0x0D,
  0x0E,
  0x0F,
  0x10,
  0x11,
  0x12,
  0x13,
  0x14,
  0x15,
  0x16,
  0x17,
  0x18,
  0x19,
  0x1A		//^Z
};

uint8_t const hid2asciiShift[] = {
  0x00, 	//0-3 empty
  0x00, 
  0x00, 
  0x00, 
  'A', 		//a..z 1..0
  'B',
  'C',
  'D',
  'E',
  'F',
  'G',
  'H',
  'I',
  'J',
  'K',
  'L',
  'M',
  'N',
  'O',
  'P',
  'Q',
  'R',
  'S',
  'T',
  'U',
  'V',
  'W',
  'X',
  'Y',
  'Z',
  '!',
  '@',
  '#',
  '$',
  '%',
  '^',
  '&',
  '*',
  '(',
  ')',
  0x0D, 	//enter
  0x1B,		//esc
  0x08,		//BS
  0x09,		//TAB
  ' ',
  '_',
  '+',
  '{',
  '}',
  '|',		// backslash
  0x00,
  ':',
  0x27,		//single quote
  '~',
  '<',
  '>',
  '?',
  0x00,		//CAPS
  0x00,		//F1
  0x00,		//F2
  0x00,		//F3
  0x00,		//F4
  0x00,		//F5
  0x00,		//F6
  0x00,		//F7
  0x00,		//F8
  0x00,		//F9
  0x00,		//F10
  0x00,		//F11
  0x00,		//F12
  0x00,		//PrtScr
  0x00,		//ScrlLock
  0x00,		//Pause/Break
  0x00,		//Ins
  0x00,		//Home
  0x00,		//PgUp
  0x00,		//Del
  0x00,		//End
  0x00,		//PgDn
  0x00,		//Right-Arr
  0x00,		//Left-Arr
  0x00,		//Down-Arr
  0x00,		//Up-Arr
  0x00,		//NUMPAD ----NumLock
  0x00,		// /
  0x00,		// *
  0x00,		// -
  0x00,		// +
  0x00,		// ???
  0x00,		// 1
  0x00,		// 2
  0x00,		// 3
  0x00,		// 4
  0x00,		// 5
  0x00,		// 6
  0x00,		// 7
  0x00,		// 8
  0x00,		// 9	
  0x00,		// Ins
  0x00,		// Del
  '>'
};

//#define HOLLOW 1
static hid_keyboard_report_t previous_report = { 0, 0, {0} }; // previous report to check key released
static hid_keyboard_report_t current_report = { 0, 0, {0} }; // current report

// look up new key in previous keys
bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  {
      return true;
    }
  }

  return false;
}



// typedef struct gamepad_mapping_t {
//     uint32_t gamepad_identifier;
//     gamepad_function_t function;
// } gamepad_mapping_t;

typedef struct {
    uint8_t dpad_offset;       // Offset in the report for the dpad
    uint8_t dp[16];
    uint8_t action_button_masks[4];   // Masks for individual buttons
    uint8_t button_offset;     // Offset in the report for buttons
    uint8_t button_masks[4];   // Masks for individual buttons
} gamepad_input_mapping_t;

typedef struct {
    uint32_t gamepad_identifier;          // Unique identifier for the gamepad
    gamepad_input_mapping_t input_map;    // Input mapping for the gamepad
} gamepad_mapping_t;

typedef struct gamepad_registration_t {
    uint16_t identifier;
    uint8_t gamepad_idx;
    uint8_t unused1;
    gamepad_input_mapping_t* gamepad_mapping;
} gamepad_registration_t;

#define DEVICE_IDENTIFIER(dev_addr,instance) (dev_addr << 8 | instance)
#define GAMEPAD_IDENTIFIER(vendor_id,product_id) (vendor_id << 16 | product_id)

gamepad_mapping_t gamepad_mappings[] = {
    {GAMEPAD_IDENTIFIER(1133, 49688), { // Logitech Rumblepad 2
        .dpad_offset = offsetof(hid_gamepad_report_t, rx),
        .dp = {
            [0] = GP_D_UP,
            [1] = GP_D_UP | GP_D_RIGHT,
            [2] = GP_D_RIGHT,
            [3] = GP_D_DOWN | GP_D_RIGHT,
            [4] = GP_D_DOWN,
            [5] = GP_D_DOWN | GP_D_LEFT,
            [6] = GP_D_LEFT,
            [7] = GP_D_UP | GP_D_LEFT,
            [8] = 0,
        },
        .action_button_masks = {128, 32, 16, 64}, // Top, Botto, Left ,right
        .button_offset = offsetof(hid_gamepad_report_t, ry),
        .button_masks = {1, 2, 16, 32} // Rear Left, Rear Right, Select, Start
    }},    
    {GAMEPAD_IDENTIFIER(121, 6), { // Tracer Glider
        
            .dpad_offset = offsetof(hid_gamepad_report_t, ry),
            .dp = {
                [0] = GP_D_UP,
                [1] = GP_D_UP | GP_D_RIGHT,
                [2] = GP_D_RIGHT,
                [3] = GP_D_DOWN | GP_D_RIGHT,
                [4] = GP_D_DOWN,
                [5] = GP_D_DOWN | GP_D_LEFT,
                [6] = GP_D_LEFT,
                [7] = GP_D_UP | GP_D_LEFT,
                [15] = 0,
            },
            .action_button_masks = {16, 64, 128, 32}, // Top, Botto, Left ,right
            .button_offset = offsetof(hid_gamepad_report_t, hat),
            .button_masks = {1, 2, 16, 32} // Rear Left, Rear Right, Select, Start
    }},   
    // {GAMEPAD_IDENTIFIER(121, 6), { // Tracer Glider
    //     .dpad_offset = offsetof(hid_gamepad_report_t, ry),
    //     .button_masks = {16, 32, 64, 128}, // Top, Right, Bottom, Left, Rear Left, Rear Right, Select, Start
    //     .button_offset = offsetof(hid_gamepad_report_t, ry),
    //     .button_masks = {1, 2, 16, 32} // Top, Right, Bottom, Left, Rear Left, Rear Right, Select, Start
    // }},
    // {GAMEPAD_IDENTIFIER(3727, 8), { // CSL PS3-Gamepad Clone
    //     .dpad_offset = offsetof(hid_gamepad_report_t, ry),
    //     .button_masks = {16, 32, 64, 128}, // Top, Right, Bottom, Left, Rear Left, Rear Right, Select, Start
    //     .button_offset = offsetof(hid_gamepad_report_t, hat),
    //     .button_masks = {1, 2, 16, 32} // Top, Right, Bottom, Left, Rear Left, Rear Right, Select, Start
    // }},
};

//         case 0 : current_state.controls |= GP_D_UP ; break;
//         case 1 : current_state.controls |= GP_D_UP | GP_D_RIGHT; break;
//         case 2 : current_state.controls |= GP_D_RIGHT; break;
//         case 3 : current_state.controls |= GP_D_DOWN | GP_D_RIGHT; break;
//         case 4 : current_state.controls |= GP_D_DOWN; break;
//         case 5 : current_state.controls |= GP_D_DOWN | GP_D_LEFT; break;
//         case 6 : current_state.controls |= GP_D_LEFT; break;
//         case 7 : current_state.controls |= GP_D_UP | GP_D_LEFT; break;
//         case 8 : current_state.controls |= 0; break; // jaja

gamepad_state_t gamepad_previous[GAMEPAD_MAX_DEVICES] = {0};

void process_gamepad_input(uint8_t gamepad_id, gamepad_input_mapping_t* input_map,hid_gamepad_report_t* report) {
    gamepad_state_t current_state = {0};
    //current_state.controls = GP_STATE_IN_USE;

    // Process D-Pad
    uint8_t dpad = *((uint8_t*)report + input_map->dpad_offset) & 0b1111;
    current_state.controls = input_map->dp[dpad];

    // Process action Buttons
    uint8_t action_buttons = *((uint8_t*)report + input_map->dpad_offset) & 0b11110000;
    for (int i = 0; i < 4; i++) {
        if (action_buttons & input_map->action_button_masks[i]) {
            current_state.buttons |= (1 << i); // Map to corresponding button bit
            
        }
    }

    uint8_t buttons = *((uint8_t*)report + input_map->button_offset);
    for (int i = 0; i < 4; i++) {
        if (buttons & input_map->button_masks[i]) {
            current_state.buttons |= (0b10000 << i); // Map to corresponding button bit
        }
    }

    gamepad_state_t* prev = &gamepad_previous[gamepad_id];

    // pressed
    gamepad_pressed[gamepad_id].buttons  |= (current_state.buttons  & ~prev->buttons);
    gamepad_pressed[gamepad_id].controls |= (current_state.controls & ~prev->controls);
    // released
    gamepad_released[gamepad_id].buttons  |= (~current_state.buttons  & prev->buttons);
    gamepad_released[gamepad_id].controls |= (~current_state.controls & prev->controls);

    // Update gamepad state
    gamepad_down[gamepad_id] = current_state;
    *prev = current_state;
}




static gamepad_mapping_t* gamepad_find_mapping(uint16_t vendor_id, uint16_t product_id){
    uint32_t gamepad_identifier = GAMEPAD_IDENTIFIER(vendor_id,product_id);
    for (int i=0;i<(sizeof(gamepad_mappings) / sizeof(gamepad_mappings[0]));i++){
        gamepad_mapping_t* mapping = &gamepad_mappings[i];
        if (mapping->gamepad_identifier == gamepad_identifier){
            return mapping;
        }
    }
    // COULDN't FIND GAMEPAD
    return NULL;
}

gamepad_registration_t gamepad_registration[GAMEPAD_MAX_DEVICES] = {0};
uint8_t gamepads_registered = 0;

void gamepad_register(uint8_t dev_addr, uint8_t instance, uint16_t vendor_id, uint16_t product_id) {
    //uint32_t gamepad_identifier = GAMEPAD_IDENTIFIER(vendor_id, product_id);

    gamepad_mapping_t* mapping = gamepad_find_mapping(vendor_id, product_id);
    if (mapping == NULL) {
        // Unknown gamepad
        return;
    }

    uint16_t device_identifier = DEVICE_IDENTIFIER(dev_addr, instance);

    for (uint8_t gamepad_id = 0; gamepad_id < GAMEPAD_MAX_DEVICES; gamepad_id++) {
        gamepad_registration_t* registration = &gamepad_registration[gamepad_id];
        if (registration->identifier != 0) {
            // Already in use
            continue;
        }
        gamepads_registered++;

        *mm_gamepad_info |= (1 << gamepad_id);

        registration->identifier = device_identifier;
        registration->gamepad_idx = gamepad_id;
        registration->gamepad_mapping = &mapping->input_map;
        //bit_set(mm_gamepad_state[registration->gamepad_idx].controls, GP_STATE_IN_USE);

        // Pass the mapping to the callback
        break;
    }
}


void gamepad_unregister(uint8_t dev_addr, uint8_t instance){
    uint16_t device_identifier = DEVICE_IDENTIFIER(dev_addr,instance);

    for (int i=0;i < GAMEPAD_MAX_DEVICES; i++){
        gamepad_registration_t* registration = &gamepad_registration[i];
        if (registration->identifier == device_identifier){
            gamepads_registered--;
            assert(gamepads_registered>=0);
            // already in use
            *mm_gamepad_info &= ~(1 << registration->gamepad_idx );
            *registration = (gamepad_registration_t){0};
            return;
        }
    }
}


static bool gamepad_find_registration(uint8_t dev_addr, uint8_t instance, gamepad_registration_t** registration){
    uint16_t device_identifier = DEVICE_IDENTIFIER(dev_addr,instance);

    for (int i=0;i < GAMEPAD_MAX_DEVICES; i++){
        *registration = &gamepad_registration[i];
        if ((*registration)->identifier == device_identifier){
            // already in use
            return true;
        }
    }
    return false;
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    #ifndef HOLLOW
    switch(tuh_hid_interface_protocol(dev_addr, instance)) {
        case HID_ITF_PROTOCOL_KEYBOARD:{
            kbd_addr = dev_addr;
            kbd_inst = instance;
            _keyboard_connected = true;
            break;
        }
        case HID_ITF_PROTOCOL_MOUSE:{
            _mouse_connected = true;
            break;
        }
        case HID_ITF_PROTOCOL_NONE: {
            uint16_t vid, pid;
            tuh_vid_pid_get(dev_addr, &vid, &pid);
            gamepad_register(dev_addr,instance,vid,pid);
            break;
        }    
    }
    // request next report
    tuh_hid_receive_report(dev_addr, instance);
    #endif
}


void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    switch(tuh_hid_interface_protocol(dev_addr, instance)) {
        case HID_ITF_PROTOCOL_KEYBOARD:{
            kbd_addr = 0;
            kbd_inst = 0;
            _keyboard_connected = false;
            break;
        }
        case HID_ITF_PROTOCOL_MOUSE:{
            _mouse_connected = false;
            break;
        }
        case HID_ITF_PROTOCOL_NONE: {
            gamepad_unregister(dev_addr,instance);
            break;
        }    
    }
}


bool keyboard_receive = false;
hid_mouse_report_t mouse_report_previous = {0};

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
  #ifndef HOLLOW

    switch(tuh_hid_interface_protocol(dev_addr, instance)) {
        case HID_ITF_PROTOCOL_NONE: {
            hid_gamepad_report_t* gamepad_report = (hid_gamepad_report_t*)report;
           *mm_gamepad = *((gamepad_t*)gamepad_report); // remove this! only for testing
            
            gamepad_registration_t* gamepad_registration;
            if (gamepad_find_registration(dev_addr,instance,&gamepad_registration)){
                process_gamepad_input(gamepad_registration->gamepad_idx, gamepad_registration->gamepad_mapping, gamepad_report);
            }
            break;
        }
        case HID_ITF_PROTOCOL_KEYBOARD: {
            current_report = *(hid_keyboard_report_t*)report;
            uint8_t keycode = *mm_keyboard_last_pressed_keycode = current_report.keycode[0];

            if ((keycode<=0x64) && (keycode>=4)) {	//if valid HID code
            if (current_report.modifier & 0x22) {			//if Shift
                *mm_keyboard_last_pressed_char = hid2asciiShift[keycode];
            } else if (current_report.modifier & 0x11) {		//if Ctrl
                if (keycode <= 0x1A)
                    *mm_keyboard_last_pressed_char = hid2asciiCtrl[keycode];
                else
                    *mm_keyboard_last_pressed_char = hid2ascii[keycode];
                } else {
                    *mm_keyboard_last_pressed_char = hid2ascii[keycode];		//no Ctrl no Shift 
                }
            };
            break;
        }
        case HID_ITF_PROTOCOL_MOUSE:{
            hid_mouse_report_t* mouse = (hid_mouse_report_t*)report;
            mouse_report.mouse_x += mouse->x;
            mouse_report.mouse_y += mouse->y;
            mouse_report.mouse_btn_state = mouse->buttons;
            mouse_report.mouse_wheel = mouse->wheel;

            mouse_report.mouse_btn_state_pressed  |= (mouse->buttons & ~mouse_report_previous.buttons);
            mouse_report.mouse_btn_state_released |= (~mouse->buttons & mouse_report_previous.buttons);

            mouse_report.mouse_x=clamp(mouse_report.mouse_x,0,320);
            mouse_report.mouse_y=clamp(mouse_report.mouse_y,0,240);

            mouse_report_previous = *mouse;
            
            //tuh_hid_receive_report(dev_addr, instance);
            break;
        }
    }
    tuh_hid_receive_report(dev_addr, instance);

#endif
}

bool io_keyboard_connected(void)
{
  return _keyboard_connected;
}

bool io_mouse_connected(void)
{
  return _mouse_connected;
}

void tuh_mount_cb(uint8_t dev_addr) {
  // application set-up
  printf("A device with address %d is mounted\r\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr) {
  // application tear-down
  printf("A device with address %d is unmounted \r\n", dev_addr);
}
#include <inttypes.h>

// //--------------------------------------------------------------------+
// // MACRO TYPEDEF CONSTANT ENUM DECLARATION
// //--------------------------------------------------------------------+
static scsi_inquiry_resp_t inquiry_resp;
//bool finished_usb_startup = false;

static FATFS msc_fatfs_volumes[CFG_TUH_DEVICE_MAX];
//static volatile bool _disk_busy[CFG_TUH_DEVICE_MAX];
static volatile bool busy[CFG_TUH_DEVICE_MAX];


//-------------------

void neo6502_usb_init(void) {
  sleep_ms(2);
  board_init();
  tusb_init();

#ifndef INCLUDE_DATA
  tuh_init(0);
#else
  finished_usb_startup = true;
#endif

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  while (utils_millis()<1000 || !finished_usb_startup){
      sleep_ms(1);
      neo6502_usb_update();
  }

}

void neo6502_usb_update(void){
  previous_report = current_report;
  tuh_task();  
}

void io_backend_init(void){
  neo6502_usb_init();
}

void io_backend_before_tick(void){
}

void io_backend_after_tick(void){
  neo6502_usb_update();
}

void io_backend_clear_state(void){
   // TODO
   
}

bool io_keyboard_is_pressed(uint8_t keycode){
  return find_key_in_report(&current_report,keycode) && !find_key_in_report(&previous_report,keycode);
}

bool io_keyboard_is_down(uint8_t keycode){
  return find_key_in_report(&current_report,keycode) && find_key_in_report(&previous_report,keycode);
}

bool io_keyboard_is_released(uint8_t keycode){
  return !find_key_in_report(&current_report,keycode) && find_key_in_report(&previous_report,keycode);
}

#endif