#ifndef _WEATHERAPI_RESPONSE_H_
#define _WEATHERAPI_RESPONSE_H_

#include <esp_system.h>

struct weatherapi_weather_name_t
{
    const char *main;
    const char *description;
};

struct weatherapi_desc_temp_t
{
    float min;
    float max;
};

#define DAILY_MAX_WEATHER_REPORT 3
struct weatherapi_desc_daily_weather_t
{
    unsigned long dt;
    unsigned long sunrise;
    unsigned long sunset;
    struct weatherapi_desc_temp_t temp;
    struct weatherapi_desc_temp_t feels_like;
    unsigned int pressure;
    u_int8_t humidity;
    u_int8_t clouds;
    unsigned int visibility;
    float wind_speed;
    u_int16_t wind_deg;
    struct weatherapi_weather_name_t weather[DAILY_MAX_WEATHER_REPORT];
};

struct weatherapi_desc_current_weather_t
{
    unsigned long dt;
    unsigned long sunrise;
    unsigned long sunset;
    float temp;
    float feels_like;
    unsigned int pressure;
    u_int8_t humidity;
    u_int8_t clouds;
    unsigned int visibility;
    float wind_speed;
    u_int16_t wind_deg;
    struct weatherapi_weather_name_t weather[DAILY_MAX_WEATHER_REPORT];
};

struct weatherapi_desc_alerts_t
{
    const char *sender_name;
    const char *event;
    const char *description;
};

#define MAX_WEATHER_ALERTS 4
#define MAX_DAYS_FORECAST 8
typedef struct weatherapi_response_t
{
    struct weatherapi_desc_current_weather_t current;
    struct weatherapi_desc_daily_weather_t daily[MAX_DAYS_FORECAST];
    struct weatherapi_desc_alerts_t alerts[MAX_WEATHER_ALERTS];
} weatherapi_response_t;

weatherapi_response_t *get_deserialized_onecall(void);

#endif
