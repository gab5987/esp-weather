/**
 * @file  epd4i2.c
 * @brief Library to work with WaveShare's 4.2 inch B/W E-Paper Display
 * @date  2023-10--12
 * @copyright Gabriel Novalski
 */
#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_netif.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <string.h>

#include "cJSON.h"
#include "config.h"
#include "hal/spi_types.h"
#include "networking.h"
#include "weatherapi_response.h"
#include "wifi.h"

#include "epd4i2.h"

void app_main()
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    // wifi_init(WIFI_SSID, WIFI_PASS);

    // get_deserialized_onecall();

    EpdDevice_t display = {
        .busy = 4,
        .rst = 16,
        .dc = 17,
        .cs = 5,
        .clk = 18,
        .din = 23,
        .spi_host = SPI3_HOST,
    };

    Epd_Initilize(&display);

    memset(display.buffer, 0, EPD_BUFFER_SIZE / 2);
    Epd_Render(&display);

    for (;;)
    {
        vTaskDelay(100);
        // Must never get here
    }
}
