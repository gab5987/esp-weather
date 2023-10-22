#ifndef _WEATHERAPI_RESPONSE_H_
#define _WEATHERAPI_RESPONSE_H_

#include <esp_system.h>

struct WeatherapiWeatherName_t
{
    int id;
    const char *main;
    const char *description;
};

struct WeatherapiDescTemp_t
{
    float min;
    float max;
};

#define DAILY_MAX_WEATHER_REPORT 3
struct WeatherapiDescDailyWeather_t
{
    unsigned long dt;
    struct WeatherapiDescTemp_t temp;
    struct WeatherapiWeatherName_t weather[DAILY_MAX_WEATHER_REPORT];
};

struct WeatherapiDescCurrentWeather_t
{
    unsigned long dt;
    unsigned long sunrise;
    unsigned long sunset;
    float temp;
    float feels_like;
    float tmax;
    float tmin;
    float uvi;
    unsigned int pressure;
    u_int8_t humidity;
    u_int8_t clouds;
    unsigned int visibility;
    float wind_speed;
    u_int16_t wind_deg;
    struct WeatherapiWeatherName_t weather[DAILY_MAX_WEATHER_REPORT];
};

struct WeatherapiDescAlerts_t
{
    const char *sender_name;
    const char *event;
    const char *description;
};

#define MAX_WEATHER_ALERTS 4
#define MAX_DAYS_FORECAST 5
typedef struct WeatherapiResponse_t
{
    struct WeatherapiDescCurrentWeather_t current;
    struct WeatherapiDescDailyWeather_t daily[MAX_DAYS_FORECAST];
    struct WeatherapiDescAlerts_t alerts[MAX_WEATHER_ALERTS];
} WeatherapiResponse_t;

#ifdef __cplusplus
extern "C"
{
#endif
    WeatherapiResponse_t *Weather_GetDeserializedOnecall(void);
#ifdef __cplusplus
}
#endif

#endif
