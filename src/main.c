#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_netif.h>
#include <nvs.h>
#include <nvs_flash.h>

#include "cJSON.h"
#include "config.h"
#include "core/wifi.h"
#include "networking.h"

void app_main()
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    wifi_init(WIFI_SSID, WIFI_PASS);

    cJSON *json = cJSON_Parse(make_weather_request());
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "timezone");
    if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        printf("Timezone: %s\n", name->valuestring);
    }
}
