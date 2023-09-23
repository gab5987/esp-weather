#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_netif.h>
#include <nvs.h>
#include <nvs_flash.h>

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

    get_deserialized_onecall();

    for (;;)
    {
        vTaskDelay(100);
        // Must never get here
    }
}
