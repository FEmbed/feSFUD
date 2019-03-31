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

#ifndef _FE_SFUD_H__
#define _FE_SFUD_H__

#include "FEmbed.h"

#include "sfud.h"

#include "feFs.h"

namespace FEmbed {
    class SFUD : public BlockImpl{
     public:
        /**
         * @brief SFUD Construct
         *
         * To initialize an SFUD device, SPI interface must be provided,
         * corresponding slice selection;
         * The default can provide chip internal information;
         * This data structure is a Wrap for sfud_flash, so some data interfaces
         *  need to refer to sfud.
         *
         * @param spi_dev SPI device used for this SFUD object.
         * @param cs SPI CS name if Flash has use CS selected.
         * @param init_chip Initial Chip structure.
         */
        SFUD(SPI *spi_dev, const char *cs = NULL,
             sfud_flash_chip *init_chip = NULL);
        virtual ~SFUD();

        SPI* getSPI() { return this->m_spi; }
        const char *getCS() { return this->m_flash_cs; }

        /**
         * SFUD can set default OPS for current FS
         * @param ops Operations object.
         */
//        void setOps(file_system_ops *ops) { this->m_fs_ops = ops;}
//        file_system_ops *getOps() { return this->m_fs_ops; }

        virtual bool erase(uint32_t addr, size_t size);
        virtual size_t read(uint32_t addr, void *data, size_t size);
        virtual size_t write(uint32_t addr, void *data, size_t size);

        uint32_t capacity() { return this->m_flash.chip.capacity; }

     private:
        SPI *m_spi;
        const char * m_flash_cs;
        // file_system_ops *m_fs_ops;
        sfud_flash m_flash;
        bool m_init;
    };
}

#endif  //_FE_SFUD_H__
