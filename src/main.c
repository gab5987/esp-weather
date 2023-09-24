#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_netif.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <stdio.h>

#include "cJSON.h"
#include "config.h"
#include "core/conversion.h"
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

    char time_bff[6] = {0};
    epoch_to_time_string(&re->current.dt, time_bff);
    printf("Atualizado em(dt): %s \n", time_bff);
    epoch_to_time_string(&re->current.sunrise, time_bff);
    printf("Nascer do sol(sunrise): %s \n", time_bff);
    epoch_to_time_string(&re->current.sunset, time_bff);
    printf("Por do sol(sunset): %s \n", time_bff);

    printf("Nuvens(clouds): %d%% \n", re->current.clouds);
    printf("Temperatura(temp): %.2f °C \n", kelvin_to_celcius(re->current.temp));
    printf("Descricao: %s \n", re->current.weather[0].description);

    for (int i = 0; i < MAX_DAYS_FORECAST; i++)
    {
        if (!re->daily[i].dt)
            break;
        printf("\n\n");
        epoch_to_time_string(&re->daily[i].sunrise, time_bff);
        printf("Nascer do sol(sunrise): %s \n", time_bff);
        epoch_to_time_string(&re->daily[i].sunset, time_bff);
        printf("Por do sol(sunset): %s \n", time_bff);

        printf("Nuvens(clouds): %d%% \n", re->daily[i].clouds);
        printf("Temperatura(temp): min -> %.2f °C | máx -> %.2f °C \n", kelvin_to_celcius(re->daily[i].temp.min),
               kelvin_to_celcius(re->daily[i].temp.max));

        printf("Sensacao térmica(feels_like): min -> %.2f °C | máx -> %.2f °C \n",
               kelvin_to_celcius(re->daily[i].feels_like.min), kelvin_to_celcius(re->daily[i].feels_like.max));
        printf("Descricao: %s \n", re->daily[i].weather[0].description);
    }

    printf("\n\n");
    for (int i = 0; i < MAX_WEATHER_ALERTS; i++)
    {
        if (re->alerts[i].event == NULL)
            break;
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
