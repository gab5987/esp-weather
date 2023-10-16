#include "cJSON.h"
#include "esp_err.h"
#include "networking.h"
#include "weatherapi_response.h"
#include <esp_log.h>
#include <stdio.h>
#include <string.h>

static const char *TAG = "apiresponse";

WeatherapiResponse_t deserialized_response = {0};

#define cJSON_get_value_number(jsonptr, variable, from_key, isfloat)                                                   \
    {                                                                                                                  \
        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonptr, from_key);                                             \
        if (item != NULL)                                                                                              \
            variable = isfloat ? (float)item->valuedouble : item->valueint;                                            \
    }

#define cJSON_get_value_string(jsonptr, variable, from_key)                                                            \
    {                                                                                                                  \
        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonptr, from_key);                                             \
        if (item->valuestring != NULL && cJSON_IsString(item))                                                         \
            variable = item->valuestring;                                                                              \
    }

inline static bool check_json_err(cJSON *obj, esp_err_t *err, const char *key)
{
    if (obj == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        ESP_LOGE(TAG, "Failed to parse JSON @ key '%s' - error_code: (%s)", key, error_ptr != NULL ? error_ptr : "");
        cJSON_Delete(obj);
        *err = ESP_ERR_NOT_FINISHED;
        return false;
    }
    return true;
}

WeatherapiResponse_t *Weather_GetDeserializedOnecall(void)
{
    esp_err_t err = ESP_OK;
    cJSON *json = NULL;

    char *net_response = Net_MakeWeatherRequest();
    if (net_response == NULL)
    {
        err = ESP_ERR_NOT_FINISHED;
        ESP_LOGE(TAG, "Failed to get api response (%s)", esp_err_to_name(err));
        goto cleanup;
    }

    json = cJSON_Parse(net_response);
    if (!check_json_err(json, &err, "root"))
        goto cleanup;

    const char *current_key = "current";
    cJSON *current = cJSON_GetObjectItemCaseSensitive(json, current_key);
    if (check_json_err(current, &err, current_key))
    {
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

        const char *weather_key = "weather";
        cJSON *json_weather_name = cJSON_GetObjectItemCaseSensitive(current, weather_key);
        if (check_json_err(json_weather_name, &err, weather_key))
        {
            for (int i = 0; i < cJSON_GetArraySize(json_weather_name); i++)
            {
                if (i == DAILY_MAX_WEATHER_REPORT)
                    break;
                cJSON *subitem = cJSON_GetArrayItem(json_weather_name, i);
                cJSON_get_value_string(subitem, deserialized_response.current.weather[i].main, "main");
                cJSON_get_value_string(subitem, deserialized_response.current.weather[i].description, "description");
            }
        }
    }

    const char *daily_key = "daily";
    cJSON *json_forecast = cJSON_GetObjectItemCaseSensitive(json, daily_key);
    if (check_json_err(json_forecast, &err, daily_key))
    {
        int max_forecast_in_array = cJSON_GetArraySize(json_forecast);
        for (int i = 0; i < MAX_DAYS_FORECAST; i++)
        {
            if (i == max_forecast_in_array)
                break;
            cJSON *subitem = cJSON_GetArrayItem(json_forecast, i);
            if (subitem == NULL)
                continue;

            cJSON_get_value_number(subitem, deserialized_response.daily[i].dt, "dt", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].sunrise, "sunrise", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].sunset, "sunset", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].pressure, "pressure", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].humidity, "humidity", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].clouds, "clouds", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].visibility, "visibility", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].wind_speed, "wind_speed", false);
            cJSON_get_value_number(subitem, deserialized_response.daily[i].wind_deg, "wind_deg", false);

            cJSON *temp = cJSON_GetObjectItemCaseSensitive(subitem, "temp");
            cJSON_get_value_number(temp, deserialized_response.daily[i].temp.max, "max", true);
            cJSON_get_value_number(temp, deserialized_response.daily[i].temp.min, "min", true);

            cJSON *feels_like = cJSON_GetObjectItemCaseSensitive(subitem, "feels_like");
            cJSON_get_value_number(feels_like, deserialized_response.daily[i].feels_like.max, "max", true);
            cJSON_get_value_number(feels_like, deserialized_response.daily[i].feels_like.min, "min", true);

            const char *weather_key = "weather";
            cJSON *json_weather_name = cJSON_GetObjectItemCaseSensitive(subitem, weather_key);
            if (check_json_err(json_weather_name, &err, weather_key))
                for (int w = 0; w < cJSON_GetArraySize(json_weather_name); w++)
                {
                    if (w == DAILY_MAX_WEATHER_REPORT)
                        break;
                    cJSON *subitem = cJSON_GetArrayItem(json_weather_name, w);
                    cJSON_get_value_string(subitem, deserialized_response.daily[i].weather[w].main, "main");
                    cJSON_get_value_string(subitem, deserialized_response.daily[i].weather[w].description,
                                           "description");
                }
        }
    }

    const char *alerts_key = "alerts";
    cJSON *json_weather_alerts = cJSON_GetObjectItemCaseSensitive(json, alerts_key);
    if (check_json_err(json_weather_alerts, &err, alerts_key))
    {
        for (int i = 0; i < cJSON_GetArraySize(json_weather_alerts); i++)
        {
            if (i == DAILY_MAX_WEATHER_REPORT)
                break;
            cJSON *subitem = cJSON_GetArrayItem(json_weather_alerts, i);
            cJSON_get_value_string(subitem, deserialized_response.alerts[i].description, "description");
            cJSON_get_value_string(subitem, deserialized_response.alerts[i].event, "event");
            cJSON_get_value_string(subitem, deserialized_response.alerts[i].sender_name, "sender_name");
        }
    }

cleanup:
    if (json != NULL)
        cJSON_free(json);
    return &deserialized_response;
}
