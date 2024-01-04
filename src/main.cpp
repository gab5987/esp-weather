
#include "renderer.h"
#include "wifi.h"

#include <esp_err.h>
#include <nvs.h>
#include <nvs_flash.h>

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

    if (Wifi_Initialize())
    {
        WeatherapiResponse_t *weather_response = Weather_GetDeserializedOnecall();
        Renderer_Render(&display, weather_response);
    }
    else
    {
        Renderer_RenderWifiError(&display);
    }

    Wifi_Disable();
    display.powerDown();

    esp_sleep_enable_timer_wakeup(150000000 /* 1.5 minutes */);
    esp_deep_sleep_start();

    /* Shall never get here */
    ESP_LOGE("MAIN", "Failed to deep sleep");
    esp_restart();
}

void loop(void)
{
}
