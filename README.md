# feSFUD
**这是一个armink/SFUD库的FastEmbedded移植，由于FastEmbedded是一个C++驱动库，所以移植文件默认放到了fe_port目录**

# 移植说明 #
## sfud_cfg.h一些说明 ##
``` c
// include FEmbed driver header.
#include "driver.h"     ///< feDriver的驱动头文件，log_d/log_i的处理在里面

#define SFUD_DEBUG_MODE
#define SFUD_DEBUG log_d
#define SFUD_INFO  log_i

#define SFUD_USING_SFDP

#define SFUD_USING_FLASH_INFO_TABLE
enum {
    SFUD_DEVICE_1ST = 0,
};

#define SFUD_DEVICE_1ST_NAME "W25Q128BV"        ///< 定义Flash芯片名称
#define SFUD_DEVICE_1ST_CS   "W25Q128BV_CS"     ///< 如果有片选信号，定义片选
#define SFUD_DEVICE_1ST_SPI  "SPI1"             ///< 定义使用的SPI

#define SFUD_FLASH_DEVICE_TABLE                                            \
{                                                                          \
    [SFUD_DEVICE_1ST] = {.name = SFUD_DEVICE_1ST_NAME, .spi.name = SFUD_DEVICE_1ST_SPI},           \
}
```
## sfud_port.cpp一些说明 ##
默认是C移植文件，这里使用c++移植文件，由于FastEmbedded的外设驱动是动态申请的，而SFUD不能传递对象名称到移植的初始化函数,故工程你需要定义如下宏(**添加这个库会默认定义**)：
> #define FE_USE_DRIVER_NAME 1

**由于SFUD不能动态释放，故被此驱动占用的SPI资源不能在工程项目中动态释放。**

``` c++
extern "C"
{
/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf, size_t read_size) {
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
    fe_delay(0);
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    switch (flash->index) {
        case SFUD_DEVICE_1ST: {
            //TODO user MUST NOT delete sfud flash object when it use for SFUD.
            FEmbed::SPI *sfud_spi = (FEmbed::SPI *)FEmbed::BaseDriver::findDriverByName(flash->spi.name);
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
```
## 更多FastEmbedded ##
> 进行中...

## 更多SFUD用法 ##
> [请转到armink/SFUD](https://github.com/armink/SFUD "armink/SFUD") 