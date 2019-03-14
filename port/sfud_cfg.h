/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2016-04-23
 */

#ifndef _SFUD_CFG_H_
#define _SFUD_CFG_H_

// include fastembedded driver header.
#include "driver.h"
#if 1
// all user define must in app.h
#include "app.h"
#else
#define SFUD_DEBUG_MODE
#define SFUD_DEBUG log_d
#define SFUD_INFO  log_i

#define SFUD_USING_SFDP

#define SFUD_USING_FLASH_INFO_TABLE
enum {
    SFUD_DEVICE_1ST = 0,
};

#define SFUD_DEVICE_1ST_NAME "W25Q64BV"
#define SFUD_DEVICE_1ST_CS   "W25Q64BV_CS"
#define SFUD_DEVICE_1ST_SPI  "SPI1"

/*| name | mf_id | type_id | capacity_id | capacity | write_mode | erase_gran | erase_gran_cmd |*/
#define SFUD_FLASH_DEVICE_TABLE                                            \
{                                                                          \
    [SFUD_DEVICE_1ST] = {.name = SFUD_DEVICE_1ST_NAME, .spi.name = SFUD_DEVICE_1ST_SPI, .chip = \
            {"W25Q64BV", SFUD_MF_ID_WINBOND, 0x40, 0x17, 8L*1024L*1024L, SFUD_WM_PAGE_256B, 4096, 0x20},  \
                        },           \
}
#endif
#endif /* _SFUD_CFG_H_ */
