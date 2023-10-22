#include <esp_err.h>
#include <nvs.h>
#include <nvs_flash.h>

#include "renderer.h"
#include "wifi.h"

GxIO_Class io(SPI, 5, 17, 16);
GxEPD_Class display(io, 16, 4);

__attribute__((noreturn)) void setup(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    display.init();

    Renderer_RenderLoading(&display);

    bool wifi_connected = Wifi_Initialize();

    if (!wifi_connected)
    {
        Renderer_RenderWifiError(&display);
        return;
    }

    WeatherapiResponse_t *re = Weather_GetDeserializedOnecall();

    Renderer_Render(&display, re);

    display.powerDown();

    for (;;)
    {
        vTaskDelay(100);
        // Must never get here
    }
}

void loop(void)
{
}