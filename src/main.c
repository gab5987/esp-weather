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
#include "weatherapi_response.h"
#include "wifi.h"
#include "renderer.h"

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

    Wifi_Initialize(WIFI_SSID, WIFI_PASS);

    WeatherapiResponse_t *re = Weather_GetDeserializedOnecall();

    Renderer_Render(&display, re);

    /*

    char time_bff[6] = {0};
    Conv_Epoch2TimeString(&re->current.dt, time_bff);
    printf("Atualizado em(dt): %s \n", time_bff);
    Conv_Epoch2TimeString(&re->current.sunrise, time_bff);
    printf("Nascer do sol(sunrise): %s \n", time_bff);
    Conv_Epoch2TimeString(&re->current.sunset, time_bff);
    printf("Por do sol(sunset): %s \n", time_bff);

    printf("Nuvens(clouds): %d%% \n", re->current.clouds);
    printf("Temperatura(temp): %.2f °C Conv_Kelvin2Celcius\n", Conv_Kelvin2Celcius(re->current.temp));
    // printf("Descricao: %s \n", re->current.weather[0].description);

    for (int i = 0; i < MAX_DAYS_FORECAST; i++)
    {
        if (!re->daily[i].dt)
            break;
        printf("\n\n");
        Conv_Epoch2TimeString(&re->daily[i].sunrise, time_bff);
        printf("Nascer do sol(sunrise): %s \n", time_bff);
        Conv_Epoch2TimeString(&re->daily[i].sunset, time_bff);
        printf("Por do sol(sunset): %s \n", time_bff);

        printf("Nuvens(clouds): %d%% \n", re->daily[i].clouds);
        printf("Temperatura(temp): min -> %.2f °C | máx -> %.2f °C \n", Conv_Kelvin2Celcius(re->daily[i].temp.min),
               Conv_Kelvin2Celcius(re->daily[i].temp.max));

        printf("Sensacao térmica(feels_like): min -> %.2f °C | máx -> %.2f °C \n",
               Conv_Kelvin2Celcius(re->daily[i].feels_like.min), Conv_Kelvin2Celcius(re->daily[i].feels_like.max));
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

    char txt[30];
    sprintf(txt, "Temperatura %d graus C", (int)Conv_Kelvin2Celcius(re->current.temp));
    Epd_DrawText(&display, 10, 10, txt, 12, false);

    Conv_Epoch2TimeString(&re->current.sunrise, time_bff);
    sprintf(txt, "Nascer do sol %s", time_bff);
    Epd_DrawText(&display, 10, 30, txt, 12, false);

    Conv_Epoch2TimeString(&re->current.sunset, time_bff);
    sprintf(txt, "Por do sol %s", time_bff);
    Epd_DrawText(&display, 10, 50, txt, 12, false);

    */

    for (;;)
    {
        vTaskDelay(100);
        // Must never get here
    }
}
