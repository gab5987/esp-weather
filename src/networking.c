#include <ctype.h>
#include <esp_event.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_tls.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "config.h"
#include "networking.h"

#define MAX_HTTP_RECV_BUFFER 1024 * 10
static const char *TAG = "network";

const char *OPEN_WEATHER_API_BASE_URI = "api.openweathermap.org";

char local_response_buffer[MAX_HTTP_RECV_BUFFER + 1] = {0};

esp_err_t Net_HttpEventHandler(esp_http_client_event_t *evt)
{
    static int index;
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
        index = 0;
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        memcpy(local_response_buffer + index, evt->data, evt->data_len);
        index += evt->data_len;
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }
    return ESP_OK;
}

char *Net_MakeWeatherRequest(void)
{
    memset(local_response_buffer, 0, MAX_HTTP_RECV_BUFFER);
    esp_http_client_config_t config = {
        .host = OPEN_WEATHER_API_BASE_URI,
        .path = "/data/3.0/onecall",
        .query = "lat=-27.5752&lon=-48.4326&exclude=minutely,hourly&appid=5b3f53c4806893d97a2b5020a211e974&"
                 "lang=pt_br",
        .event_handler = Net_HttpEventHandler,
        .disable_auto_redirect = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %lld", esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    else
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    esp_http_client_cleanup(client);

    return local_response_buffer;
}
