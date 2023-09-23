#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_netif.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <stdio.h>

#include "cJSON.h"
#include "config.h"
#include "core/networking.h"
#include "core/weatherapi_response.h"
#include "core/wifi.h"

void app_main()
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    wifi_init(WIFI_SSID, WIFI_PASS);

    weatherapi_response_t *re = get_deserialized_onecall();

    printf("dt: %ld \n", re->current.dt);
    printf("clouds: %d \n", re->current.clouds);
    printf("temp: %.2f \n", re->current.temp);
    printf("WatherDesc: %s \n", re->current.weather[0].description);

    printf("\n\n");
    for (int i = 0; i < MAX_WEATHER_ALERTS; i++)
    {
        printf("sender_name: %s\n", re->alerts[i].sender_name);
        printf("event: %s\n", re->alerts[i].event);
        printf("description: %s\n", re->alerts[i].description);
    }

    for (;;)
    {
        vTaskDelay(100);
        // Must never get here
    }
}
