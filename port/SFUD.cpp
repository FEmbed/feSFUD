/* FastEmbedded Microcontroller Library
 * Copyright (c) 2018-2028 Gene Kong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "SFUD.hpp"

#include <cstring>

/******************************************************************************
 * This is a necessary code migration for SFUD.
 *******************************************************************************/
extern "C"
{
static sfud_err spi_write_read(const sfud_spi *spi,
                               const uint8_t *write_buf,
                               size_t write_size,
                               uint8_t *read_buf,
                               size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    FEmbed::SFUD * sfud = static_cast<FEmbed::SFUD *>(spi->user_data);
    if(sfud != NULL && sfud->getSPI() != NULL)
    {
        if(sfud->getSPI()->readAfterWrite(sfud->getCS(), (uint8_t*) write_buf, write_size,
                (uint8_t*) read_buf, read_size) < 0)
        {
            return SFUD_ERR_READ;
        }
    }
    else
        return SFUD_ERR_NOT_FOUND;
    return result;
}

static void spi_lock(const sfud_spi *spi) {
    FEmbed::SFUD * sfud = static_cast<FEmbed::SFUD *>(spi->user_data);
    if(sfud != NULL && sfud->getSPI() != NULL)
    {
        sfud->getSPI()->lock();
    }
}

static void spi_unlock(const sfud_spi *spi) {
    FEmbed::SFUD * sfud = static_cast<FEmbed::SFUD *>(spi->user_data);
    if(sfud != NULL && sfud->getSPI() != NULL)
    {
        sfud->getSPI()->unlock();
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
    return SFUD_SUCCESS;
}

}
/******************************************************************************
 * End of SFUD code.
 *******************************************************************************/
namespace FEmbed
{
    SFUD::SFUD(SPI* spi_dev,
               const char *cs,
               sfud_flash_chip *init_chip)
    {
        this->m_spi = spi_dev;
        this->m_flash_cs = cs;
        memset(&this->m_flash, 0, sizeof(this->m_flash));
        if(init_chip != NULL)
        {
            memcpy(&this->m_flash.chip, init_chip, sizeof(sfud_flash_chip));
        }
        // All sfud instance name is same.
        this->m_flash.name = "FE_SFUD";
        this->m_flash.spi.wr = spi_write_read;
        this->m_flash.spi.lock = spi_lock;
        this->m_flash.spi.unlock = spi_unlock;
        this->m_flash.spi.user_data = this;
        /* about 100 microsecond delay */
        this->m_flash.retry.delay = retry_delay_100us;
        /* adout 60 seconds timeout */
        this->m_flash.retry.times = 60 * 1000;
        if(sfud_device_init(&this->m_flash) != SFUD_SUCCESS)
        {
            this->m_init = false;
        }
        else
        {
            this->m_init = true;
        }
    }

    SFUD::~SFUD()
    {

    }

    bool SFUD::erase(uint32_t addr, size_t size)
    {
        if(this->m_init)
        {
            sfud_err err =
                    sfud_erase(&this->m_flash, addr, size);
            if(err == SFUD_SUCCESS)
                return true;
        }
        return false;
    }

    size_t SFUD::read(uint32_t addr, void* data, size_t size)
    {
        if(this->m_init)
        {
            sfud_err err = sfud_read(&this->m_flash,
                                     addr, size, (uint8_t *)data);
            if(err == SFUD_SUCCESS)
                return size;
        }

        return 0;
    }

    size_t SFUD::write(uint32_t addr, void* data, size_t size)
    {
        if(this->m_init)
        {
            sfud_err err = sfud_write(&this->m_flash,
                                     addr, size, (uint8_t *)data);
            if(err == SFUD_SUCCESS)
                return size;
        }

        return 0;
    }
}
