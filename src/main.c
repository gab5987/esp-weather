#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_netif.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "conversion.h"
#include "hal/spi_types.h"
#include "networking.h"
#include "renderer.h"
#include "weatherapi_response.h"
#include "wifi.h"

#include "cJSON.h"
#include "epd4i2.h"

void app_main()
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

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

    Renderer_RenderLoading(&display);

    bool wifi_connected = Wifi_Initialize(WIFI_SSID, WIFI_PASS);

    if (!wifi_connected)
    {
        Renderer_RenderWifiError(&display);
        return;
    }

    WeatherapiResponse_t *re = Weather_GetDeserializedOnecall();

    Renderer_Render(&display, re);

    for (;;)
    {
        vTaskDelay(100);
        // Must never get here
    }
}
