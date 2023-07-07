/*
 * Copyright (c) Riverdi Sp. z o.o. sp. k. <contact@riverdi.com>
 * Copyright (c) Skalski Embedded Technologies <contact@lukasz-skalski.com>
 */

#include "platform.h"

spi_device_handle_t spi;

/*
 * platform_init()
 */
bool_t
platform_init (Gpu_HalInit_t *halinit)
{
  /* intentionally empty */
  return TRUE;
}

/*
 * platform_sleep_ms()
 */
void
platform_sleep_ms (uint32_t ms)
{
  vTaskDelay(ms/portTICK_PERIOD_MS);
}

/*
 * platform_spi_init()
 */
bool_t
platform_spi_init (Gpu_Hal_Context_t *host)
{
  esp_err_t ret;

  spi_bus_config_t buscfg = {
    .miso_io_num   = GPIO_MISO,
    .mosi_io_num   = GPIO_MOSI,
    .sclk_io_num   = GPIO_SCLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1
  };

  spi_device_interface_config_t devcfg = {
    .clock_speed_hz = SPI_SPEED_HZ,
    .mode           = SPI_MODE,
    .spics_io_num   = -1,
    .queue_size     = 8
  };

  ret = spi_bus_initialize(SPI_DEVICE, &buscfg, SPI_DMA_CH_AUTO);
  ESP_ERROR_CHECK(ret);

  ret=spi_bus_add_device(SPI_DEVICE, &devcfg, &spi);
  ESP_ERROR_CHECK(ret);

  return TRUE;
}

/*
 * platform_spi_deinit()
 */
void
platform_spi_deinit (Gpu_Hal_Context_t *host)
{
  return;
}

/*
 * platform_spi_send_recv_byte();
 */
uchar8_t
platform_spi_send_recv_byte (Gpu_Hal_Context_t  *host,
                             uchar8_t            data,
                             uint32_t            opt)
{
  esp_err_t ret;
  spi_transaction_t t;

  memset(&t, 0, sizeof(t));
  t.length    = 8;
  t.tx_buffer = &data;
  t.flags     = SPI_TRANS_USE_RXDATA;

  ret = spi_device_polling_transmit(spi, &t);
  ESP_ERROR_CHECK(ret);

  return t.rx_data[0];
}

/*
 * platform_spi_send_data()
 */
uint16_t
platform_spi_send_data (Gpu_Hal_Context_t  *host,
                        uchar8_t           *data,
                        uint16_t            size,
                        uint32_t            opt)
{
  esp_err_t ret;
  spi_transaction_t t;

  memset(&t, 0, sizeof(t));
  t.length    = size*8;
  t.tx_buffer = data;

  ret = spi_device_polling_transmit(spi, &t);
  ESP_ERROR_CHECK(ret);

  return size;
}

/*
 * platform_spi_recv_data()
 */
void
platform_spi_recv_data (Gpu_Hal_Context_t  *host,
                        uchar8_t           *data,
                        uint16_t            size,
                        uint32_t            opt)
{
  /* intentionally empty */
  return;
}

/*
 * platform_gpio_init()
 */
bool_t
platform_gpio_init (Gpu_Hal_Context_t *host,
                    gpio_name          ngpio)
{
  gpio_set_direction(ngpio, GPIO_MODE_OUTPUT);
  return TRUE;
}

/*
 * platform_gpio_value()
 */
bool_t
platform_gpio_value (Gpu_Hal_Context_t  *host,
                     gpio_name           ngpio,
                     gpio_val            vgpio)
{
  gpio_set_level(ngpio, vgpio);
  return TRUE;
}
