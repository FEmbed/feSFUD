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
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <assert.h>
#include "FEmbed.h"

#if (FASTEMBEDDED_USE_DRIVER_NAME == 0)
#error feSFUD need set FE_USE_DRIVER_NAME = 1!
#endif

#if USE_OSLIB
#include "osTask.h"
#endif

////////////////////////////////////////////////////////////////////////////////
///                         SFUD CFG TEMPL
/// .name: if your flash has cs pin, please use cs pin object name.
/// .spi.name: please use your spi object name.
////////////////////////////////////////////////////////////////////////////////
/*
#define SFUD_USING_FLASH_INFO_TABLE
enum {
    SFUD_DEVICE_1ST = 0,
};

#define SFUD_FLASH_DEVICE_TABLE                                            \
{                                                                          \
    [SFUD_DEVICE_1ST] = {.name = "W25Q128", .spi.name = "SPI1"},           \
}
*/
extern "C"
{
/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    FEmbed::SPI * sfud_spi = static_cast<FEmbed::SPI *>(spi->user_data);
    if(sfud_spi != NULL)
    {
        if(sfud_spi->readAfterWrite(SFUD_DEVICE_1ST_CS, (uint8_t*) write_buf, write_size,
                (uint8_t*) read_buf, read_size) <0)
        {
            return SFUD_ERR_READ;
        }
    }
    else
        return SFUD_ERR_NOT_FOUND;
    return result;
}

static void spi_lock(const sfud_spi *spi) {
    FEmbed::SPI * sfud_spi = static_cast<FEmbed::SPI *>(spi->user_data);
    if(sfud_spi != NULL)
    {
        sfud_spi->lock();
    }
}

static void spi_unlock(const sfud_spi *spi) {
    FEmbed::SPI * sfud_spi = static_cast<FEmbed::SPI *>(spi->user_data);
    if(sfud_spi != NULL)
    {
        sfud_spi->unlock();
    }
}

static void retry_delay_100us(void) {
#if USE_OSLIB
    FEmbed::osDelay(1);
#else
    fe_delay(0);
#endif
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    switch (flash->index) {
        case SFUD_DEVICE_1ST: {
            //TODO user MUST NOT delete sfud flash object when it use for SFUD.
            FEmbed::SPI *sfud_spi =
                    (FEmbed::SPI *)FEmbed::BaseDriver::findDriverByName(flash->spi.name);
            assert(sfud_spi);

            /* 同步 Flash 移植所需的接口及数据 */
            flash->spi.wr = spi_write_read;
            flash->spi.lock = spi_lock;
            flash->spi.unlock = spi_unlock;
            flash->spi.user_data = sfud_spi;
            /* about 100 microsecond delay */
            flash->retry.delay = retry_delay_100us;
            /* adout 60 seconds timeout */
            flash->retry.times = 60 * 1000;
            break;
        }
    }
    return result;
}

}
