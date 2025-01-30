#ifdef PICO_NEO6502
#ifndef INCLUDE_DATA
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
#include "../../../ng_assets.h"

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

#include "pico/flash.h"
#include "hardware/flash.h"
#include "hardware/sync.h"


void tuh_mount_cb(uint8_t dev_addr) {
  // application set-up
  printf("A device with address %d is mounted\r\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr) {
  // application tear-down
  printf("A device with address %d is unmounted \r\n", dev_addr);
}
#include <inttypes.h>

static scsi_inquiry_resp_t inquiry_resp;
extern bool finished_usb_startup;

static FATFS msc_fatfs_volumes[CFG_TUH_DEVICE_MAX];
//static volatile bool _disk_busy[CFG_TUH_DEVICE_MAX];
static volatile bool busy[CFG_TUH_DEVICE_MAX];

extern uint8_t __flash_binary_end;
#ifndef INCLUDE_DATA
extern void* assets_data;
#endif
extern size_t assets_size;

int error = 0;


bool inquiry_complete_cb(uint8_t dev_addr, tuh_msc_complete_data_t const * cb_data)
{
  msc_cbw_t const* cbw = cb_data->cbw;
  msc_csw_t const* csw = cb_data->csw;

  if (csw->status != 0)
  {
    return false;
  }
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    char drive_path[3] = "0:";
    drive_path[0] += dev_addr;
    FRESULT result = f_mount(&msc_fatfs_volumes[dev_addr], drive_path, 1);
    if (result != FR_OK) {
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


    uint8_t filedata[4096];

    if (f_stat("/assets.dat", &fno) != FR_OK) {
        finished_usb_startup = true;
        return false;
    }

    DWORD file_size = fno.fsize; // Get the file size  
    // Allocate memory for the file data with padding
    assets_size = file_size;



    FIL file;
    if (f_open(&file, "/assets.dat", FA_READ) != FR_OK) {
        // Error handling
        return false;
    }

    uintptr_t p = (uintptr_t)&__flash_binary_end;
    //uintptr_t p = (uintptr_t)0x10015b84;
    assets_data = (void *)((p + 4096) & 0xFFFFF000); // Align to 4KB
    int flashOffset = (uintptr_t)assets_data - XIP_BASE;

    UINT bytes_read;
    

    while (file_size>0){
        DWORD load_bytes = min(file_size,4096);
        if (f_read(&file, filedata, load_bytes, &bytes_read) != FR_OK) {
            // Error handling
            f_close(&file);
            return false;
        }
        file_size-=bytes_read;    

        uint32_t saved_interrupts = save_and_disable_interrupts();
        flash_range_erase(flashOffset, 4096);
        flash_range_program(flashOffset, filedata, 4096);
        restore_interrupts(saved_interrupts);

        flashOffset+=4096;

    }
    

    f_close(&file);



    int b=0;
    // res = f_opendir(&dir, "/"); // Open the root directory
    // if (res != FR_OK) {
    //     printf("Failed to open directory: %d\n", res);
    //     return;
    // }

    // while (1) {
    //     res = f_readdir(&dir, &fno); // Read a directory item
    //     if (res != FR_OK || fno.fname[0] == 0) break; // Break on error or end of directory

    //     // Print the name of the file or directory
    //     if (fno.fattrib & AM_DIR) {
    //         printf("Directory: %s\n", fno.fname); // Print directory names
    //     } else {
    //         printf("File: %s\n", fno.fname); // Print file names
    //     }
    // }

   // f_closedir(&dir);


    finished_usb_startup = true;

  return true;
}

//------------- IMPLEMENTATION -------------//
void tuh_msc_mount_cb(uint8_t dev_addr)
{
  printf("A MassStorage device is mounted\r\n");

#ifndef INCLUDE_DATA
  uint8_t const lun = 0;
  tuh_msc_inquiry(dev_addr, lun, &inquiry_resp, inquiry_complete_cb, 0);
#else
    halt(1);
#endif  
}

void tuh_msc_umount_cb(uint8_t dev_addr)
{
  (void) dev_addr;
  printf("A MassStorage device is unmounted\r\n");
}

static void wait_for_disk_io(BYTE pdrv) {
    while (busy[pdrv]) {
        tuh_task();
    }
}

static bool disk_io_complete(uint8_t dev_addr, tuh_msc_complete_data_t const *cb_data) {
    (void)cb_data;
    busy[dev_addr] = false;
    return true;
}

DSTATUS disk_status(BYTE pdrv) {
    uint8_t dev_addr = pdrv;
    return tuh_msc_mounted(dev_addr) ? 0 : STA_NODISK;
}

DSTATUS disk_initialize(BYTE pdrv) {
    (void)(pdrv);
    return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    uint8_t const dev_addr = pdrv;
    uint8_t const lun = 0;
    busy[pdrv] = true;
    tuh_msc_read10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete, 0);
    wait_for_disk_io(pdrv);
    return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    uint8_t const dev_addr = pdrv;
    uint8_t const lun = 0;
    busy[pdrv] = true;
    tuh_msc_write10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete, 0);
    wait_for_disk_io(pdrv);
    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    uint8_t const dev_addr = pdrv;
    uint8_t const lun = 0;
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
            *((DWORD *)buff) = (WORD)tuh_msc_get_block_count(dev_addr, lun);
            return RES_OK;
        case GET_SECTOR_SIZE:
            *((WORD *)buff) = (WORD)tuh_msc_get_block_size(dev_addr, lun);
            return RES_OK;
        case GET_BLOCK_SIZE:
            *((DWORD *)buff) = 1;  // 1 sector
            return RES_OK;
        default:
            return RES_PARERR;
    }
}


//-------------------
#endif
#endif