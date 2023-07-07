/*
 * Copyright (c) Riverdi Sp. z o.o. sp. k. <contact@riverdi.com>
 * Copyright (c) Skalski Embedded Technologies <contact@lukasz-skalski.com>
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/******************************************************************************/

/* type definitions for 'riverdi-eve-lib' library */
#define TRUE		(1)
#define FALSE		(0)

typedef char           bool_t;
typedef char           char8_t;
typedef unsigned char  uchar8_t;
typedef signed char    schar8_t;
typedef float          float_t;

/* esp peripherals */
#include "driver/gpio.h"
#include "driver/spi_master.h"

/* predefined Riverdi modules */
#include "modules.h"

/* 'riverdi-eve-lib' */
#include "Gpu_Hal.h"
#include "Gpu.h"
#include "CoPro_Cmds.h"
#include "Hal_Utils.h"

/******************************************************************************/

#define SPI_MODE      0
#define SPI_BITS      8
#define SPI_SPEED_HZ	20000000

#ifdef CONFIG_IDF_TARGET_ESP32

#define SPI_DEVICE    HSPI_HOST
typedef enum {
  GPIO_MISO = 25,
  GPIO_MOSI = 23,
  GPIO_SCLK = 19,
  GPIO_CS   = 21,
  GPIO_PD   = 18,
  GPIO_INT  = 5
} gpio_name;

#elif defined CONFIG_IDF_TARGET_ESP32S2 || \
              CONFIG_IDF_TARGET_ESP32S3

#define SPI_DEVICE    SPI2_HOST
typedef enum {
  GPIO_MISO = 37,
  GPIO_MOSI = 35,
  GPIO_SCLK = 36,
  GPIO_CS   = 4,
  GPIO_PD   = 5,
  GPIO_INT  = 6
} gpio_name;

#elif defined CONFIG_IDF_TARGET_ESP32C3 || \
              CONFIG_IDF_TARGET_ESP32C2 || \
              CONFIG_IDF_TARGET_ESP32C6

#define SPI_DEVICE    SPI2_HOST
typedef enum {
  GPIO_MISO = 2
  GPIO_MOSI = 7,
  GPIO_SCLK = 6,
  GPIO_CS   = 9,
  GPIO_PD   = 4,
  GPIO_INT  = 5
} gpio_name;

#elif defined CONFIG_IDF_TARGET_ESP32H2

#define SPI_DEVICE    SPI2_HOST
typedef enum {
  GPIO_MISO = 0
  GPIO_MOSI = 5,
  GPIO_SCLK = 4,
  GPIO_CS   = 10,
  GPIO_PD   = 11,
  GPIO_INT  = 12
} gpio_name;

#endif

typedef enum {
  GPIO_HIGH = 1,
  GPIO_LOW  = 0
} gpio_val;

/******************************************************************************/

bool_t
platform_init                 (Gpu_HalInit_t*);

void
platform_sleep_ms             (uint32_t);

bool_t
platform_spi_init             (Gpu_Hal_Context_t*);

void
platform_spi_deinit           (Gpu_Hal_Context_t*);

uchar8_t
platform_spi_send_recv_byte   (Gpu_Hal_Context_t*,
                               uchar8_t,
                               uint32_t);

uint16_t
platform_spi_send_data        (Gpu_Hal_Context_t*,
                               uchar8_t*,
                               uint16_t,
                               uint32_t);

void
platform_spi_recv_data        (Gpu_Hal_Context_t*,
                               uchar8_t*,
                               uint16_t,
                               uint32_t);

bool_t
platform_gpio_init            (Gpu_Hal_Context_t*,
                               gpio_name);

bool_t
platform_gpio_value           (Gpu_Hal_Context_t*,
                               gpio_name,
                               gpio_val);

/******************************************************************************/

#endif /*_PLATFORM_H_*/
