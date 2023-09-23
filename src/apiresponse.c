#include "cJSON.h"
#include "core/networking.h"
#include "core/weatherapi_response.h"
#include "esp_err.h"
#include <stdio.h>
#include <stdlib.h>

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
        struct weatherapi_desc_current_weather_t current_weather = {0};
        cJSON *current = cJSON_GetObjectItemCaseSensitive(json, "current");

        cJSON_get_value_number(current, current_weather.dt, "dt", false);
        cJSON_get_value_number(current, current_weather.clouds, "clouds", false);
        cJSON_get_value_number(current, current_weather.temp, "temp", true);

        struct weatherapi_weather_name_t weather_name[3];
        cJSON *json_weather_name = cJSON_GetObjectItemCaseSensitive(current, "weather");
        for (int i = 0; i < cJSON_GetArraySize(json_weather_name); i++)
        {
            if (i == DAILY_MAX_WEATHER_REPORT)
                break;
            cJSON *subitem = cJSON_GetArrayItem(json_weather_name, i);
            cJSON_get_value_string(subitem, weather_name[i].main, "main");
            cJSON_get_value_string(subitem, weather_name[i].description, "description");
        }

        memcpy(&deserialized_response.current.weather, &weather_name, sizeof(struct weatherapi_weather_name_t));
    }

    printf("dt: %ld \n", current_weather.dt);
    printf("clouds: %d \n", current_weather.clouds);
    printf("temp: %.2f \n", current_weather.temp);
    printf("WatherDesc: %s \n", weather_name[0].description);

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

