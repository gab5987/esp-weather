#include "cJSON.h"
#include "core/networking.h"
#include "core/weatherapi_response.h"
#include "esp_err.h"
#include <stdio.h>
#include <string.h>

weatherapi_response_t deserialized_response = {0};

#define cJSON_get_value_number(jsonptr, variable, from_key, isfloat)                                                   \
    {                                                                                                                  \
        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonptr, from_key);                                             \
        variable = isfloat ? (float)item->valuedouble : item->valueint;                                                \
    }

#define cJSON_get_value_string(jsonptr, variable, from_key)                                                            \
    {                                                                                                                  \
        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonptr, from_key);                                             \
        if (item->valuestring != NULL && cJSON_IsString(item))                                                         \
            variable = item->valuestring;                                                                              \
    }

weatherapi_response_t *get_deserialized_onecall(void)
{
    esp_err_t err = ESP_OK;
    cJSON *json = NULL;

    char *net_response = make_weather_request();
    if (net_response == NULL)
    {
        err = ESP_ERR_NOT_FINISHED;
        goto cleanup;
    }

    json = cJSON_Parse(net_response);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        err = ESP_ERR_NOT_FINISHED;
        goto cleanup;
    }

    {
        cJSON *current = cJSON_GetObjectItemCaseSensitive(json, "current");
        cJSON_get_value_number(current, deserialized_response.current.dt, "dt", false);
        cJSON_get_value_number(current, deserialized_response.current.sunrise, "sunrise", false);
        cJSON_get_value_number(current, deserialized_response.current.sunset, "sunset", false);
        cJSON_get_value_number(current, deserialized_response.current.temp, "temp", true);
        cJSON_get_value_number(current, deserialized_response.current.feels_like, "feels_like", true);
        cJSON_get_value_number(current, deserialized_response.current.pressure, "pressure", false);
        cJSON_get_value_number(current, deserialized_response.current.humidity, "humidity", false);
        cJSON_get_value_number(current, deserialized_response.current.clouds, "clouds", false);
        cJSON_get_value_number(current, deserialized_response.current.visibility, "visibility", false);
        cJSON_get_value_number(current, deserialized_response.current.wind_speed, "wind_speed", false);
        cJSON_get_value_number(current, deserialized_response.current.wind_deg, "wind_deg", false);

        struct weatherapi_weather_name_t weather_name[DAILY_MAX_WEATHER_REPORT];
        cJSON *json_weather_name = cJSON_GetObjectItemCaseSensitive(current, "weather");
        for (int i = 0; i < cJSON_GetArraySize(json_weather_name); i++)
        {
            if (i == DAILY_MAX_WEATHER_REPORT)
                break;
            cJSON *subitem = cJSON_GetArrayItem(json_weather_name, i);
            cJSON_get_value_string(subitem, weather_name[i].main, "main");
            cJSON_get_value_string(subitem, weather_name[i].description, "description");
        }

        memcpy(&deserialized_response.current.weather, &weather_name,
               DAILY_MAX_WEATHER_REPORT * sizeof(struct weatherapi_weather_name_t));
    }

    {
        struct weatherapi_desc_alerts_t weather_alerts[MAX_WEATHER_ALERTS];
        cJSON *json_weather_alerts = cJSON_GetObjectItemCaseSensitive(json, "alerts");
        for (int i = 0; i < cJSON_GetArraySize(json_weather_alerts); i++)
        {
            if (i == DAILY_MAX_WEATHER_REPORT)
                break;
            cJSON *subitem = cJSON_GetArrayItem(json_weather_alerts, i);
            cJSON_get_value_string(subitem, weather_alerts[i].description, "description");
            cJSON_get_value_string(subitem, weather_alerts[i].event, "event");
            cJSON_get_value_string(subitem, weather_alerts[i].sender_name, "sender_name");
        }

        memcpy(&deserialized_response.alerts, &weather_alerts,
               MAX_WEATHER_ALERTS * sizeof(struct weatherapi_desc_alerts_t));
    }

cleanup:
    /*
      if (net_response != NULL)
          free(net_response);
      if (json != NULL)
          free(json);
    */
    if (err != ESP_OK)
        return NULL;
    return &deserialized_response;
}

