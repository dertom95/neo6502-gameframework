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


void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
  #ifndef HOLLOW
  switch(tuh_hid_interface_protocol(dev_addr, instance)) {
    case HID_ITF_PROTOCOL_KEYBOARD:
      kbd_addr = dev_addr;
      kbd_inst = instance;
      tuh_hid_receive_report(dev_addr, instance);
      _keyboard_connected = true;
    break;
    
    case HID_ITF_PROTOCOL_MOUSE:
      tuh_hid_receive_report(dev_addr, instance);
      _mouse_connected = true;
    break;
  }
  #endif
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
}



bool keyboard_receive = false;

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
  #ifndef HOLLOW

  switch(tuh_hid_interface_protocol(dev_addr, instance)) {
    case HID_ITF_PROTOCOL_KEYBOARD: {
      tuh_hid_receive_report(dev_addr, instance);
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
      tuh_hid_receive_report(dev_addr, instance);

      hid_mouse_report_t* mouse = (hid_mouse_report_t*)report;
      *mm_mouse_x += mouse->x;
      *mm_mouse_y += mouse->y;
      *mm_mouse_btn_state = mouse->buttons;
      *mm_mouse_wheel = mouse->wheel;

      if (*mm_mouse_x < 0) *mm_mouse_x=0;
      if (*mm_mouse_x > 320) *mm_mouse_x=320;
      if (*mm_mouse_y < 0) *mm_mouse_y=0;
      if (*mm_mouse_y > 240)*mm_mouse_y=240;
      
      //tuh_hid_receive_report(dev_addr, instance);
      break;
    }
  }

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

/*bool inquiry_complete_cb(uint8_t dev_addr, tuh_msc_complete_data_t const * cb_data)
{
  msc_cbw_t const* cbw = cb_data->cbw;
  msc_csw_t const* csw = cb_data->csw;

  if (csw->status != 0)
  {
    return false;
  }


    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);
   // CONWriteString("USB Key found %04x %04x\r",vid,pid);

    char drive_path[3] = "0:";
    drive_path[0] += dev_addr;
    FRESULT result = f_mount(&msc_fatfs_volumes[dev_addr], drive_path, 1);
    if (result != FR_OK) {
        // CONWriteString("MSC filesystem mount failed\r\n");
        return false;
    }

    char s[2];
    if (FR_OK != f_getcwd(s, 2)) {
        f_chdrive(drive_path);
        f_chdir("/");
        
    }

    FILINFO fno;      // File information object
    DIR dir;          // Directory object
    FRESULT res;      // Result code

    res = f_opendir(&dir, "/"); // Open the root directory
    if (res != FR_OK) {
        printf("Failed to open directory: %d\n", res);
        return;
    }

    while (1) {
        res = f_readdir(&dir, &fno); // Read a directory item
        if (res != FR_OK || fno.fname[0] == 0) break; // Break on error or end of directory

        // Print the name of the file or directory
        if (fno.fattrib & AM_DIR) {
            printf("Directory: %s\n", fno.fname); // Print directory names
        } else {
            printf("File: %s\n", fno.fname); // Print file names
        }
    }

    f_closedir(&dir);

    finished_usb_startup = true;

  return true;
}
*/
// //------------- IMPLEMENTATION -------------//
// void tuh_msc_mount_cb(uint8_t dev_addr)
// {
//   printf("A MassStorage device is mounted\r\n");

//   uint8_t const lun = 0;
//   tuh_msc_inquiry(dev_addr, lun, &inquiry_resp, inquiry_complete_cb, 0);
// }

// void tuh_msc_umount_cb(uint8_t dev_addr)
// {
//   (void) dev_addr;
//   printf("A MassStorage device is unmounted\r\n");
// }

// static void wait_for_disk_io(BYTE pdrv) {
//     while (busy[pdrv]) {
//         tuh_task();
//     }
// }

// static bool disk_io_complete(uint8_t dev_addr, tuh_msc_complete_data_t const *cb_data) {
//     (void)cb_data;
//     busy[dev_addr] = false;
//     return true;
// }

// DSTATUS disk_status(BYTE pdrv) {
//     uint8_t dev_addr = pdrv;
//     return tuh_msc_mounted(dev_addr) ? 0 : STA_NODISK;
// }

// DSTATUS disk_initialize(BYTE pdrv) {
//     (void)(pdrv);
//     return 0;
// }

// DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
//     uint8_t const dev_addr = pdrv;
//     uint8_t const lun = 0;
//     busy[pdrv] = true;
//     tuh_msc_read10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete, 0);
//     wait_for_disk_io(pdrv);
//     return RES_OK;
// }

// DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
//     uint8_t const dev_addr = pdrv;
//     uint8_t const lun = 0;
//     busy[pdrv] = true;
//     tuh_msc_write10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete, 0);
//     wait_for_disk_io(pdrv);
//     return RES_OK;
// }

// DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
//     uint8_t const dev_addr = pdrv;
//     uint8_t const lun = 0;
//     switch (cmd) {
//         case CTRL_SYNC:
//             return RES_OK;
//         case GET_SECTOR_COUNT:
//             *((DWORD *)buff) = (WORD)tuh_msc_get_block_count(dev_addr, lun);
//             return RES_OK;
//         case GET_SECTOR_SIZE:
//             *((WORD *)buff) = (WORD)tuh_msc_get_block_size(dev_addr, lun);
//             return RES_OK;
//         case GET_BLOCK_SIZE:
//             *((DWORD *)buff) = 1;  // 1 sector
//             return RES_OK;
//         default:
//             return RES_PARERR;
//     }
// }


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