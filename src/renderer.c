#include "renderer.h"
#include "conversion.h"
#include "epd4i2.h"
#include "icons/icons_24x24.h"
#include "icons/icons_64x64.h"
#include <stdio.h>
#include <string.h>

#define HARSH_MAX 44

typedef struct
{
    int id;
    unsigned const char *ico;
} IconsKeyValuePair;

IconsKeyValuePair day_icons_harshtable[HARSH_MAX] = {
    // Group 2xx: Thunderstorm
    {.id = 200, .ico = wi_day_snow_thunderstorm_64x64},
    {.id = 201, .ico = wi_day_thunderstorm_64x64},
    {.id = 202, .ico = wi_day_thunderstorm_64x64},

    {.id = 210, .ico = wi_day_lightning_64x64},
    {.id = 211, .ico = wi_lightning_64x64},
    {.id = 212, .ico = wi_lightning_64x64},
    {.id = 221, .ico = wi_lightning_64x64},

    {.id = 230, .ico = wi_day_sleet_storm_64x64},
    {.id = 231, .ico = wi_day_sleet_storm_64x64},
    {.id = 232, .ico = wi_day_sleet_storm_64x64},

    // Group 3xx: Drizzle
    {.id = 300, .ico = wi_day_snow_wind_64x64},
    {.id = 301, .ico = wi_day_sleet_64x64},
    {.id = 302, .ico = wi_day_sprinkle_64x64},

    {.id = 310, .ico = wi_day_sprinkle_64x64},
    {.id = 311, .ico = wi_day_sprinkle_64x64},
    {.id = 312, .ico = wi_day_sprinkle_64x64},

    {.id = 313, .ico = wi_day_sprinkle_64x64},
    {.id = 314, .ico = wi_day_sprinkle_64x64},
    {.id = 321, .ico = wi_day_sprinkle_64x64},

    // Group 5xx: Rain
    {.id = 500, .ico = wi_day_showers_64x64},
    {.id = 501, .ico = wi_day_rain_mix_64x64},
    {.id = 502, .ico = wi_day_rain_64x64},
    {.id = 503, .ico = wi_rain_64x64},
    {.id = 504, .ico = wi_rain_64x64},

    {.id = 511, .ico = wi_snowflake_cold_64x64},

    {.id = 520, .ico = wi_day_sprinkle_64x64},
    {.id = 521, .ico = wi_day_sprinkle_64x64},
    {.id = 522, .ico = wi_day_sprinkle_64x64},
    {.id = 531, .ico = wi_day_sprinkle_64x64},

    // Group 7xx: Atmosphere
    {.id = 701, .ico = wi_fog_64x64},
    {.id = 711, .ico = wi_smoke_64x64},
    {.id = 721, .ico = wi_smoke_64x64},

    {.id = 731, .ico = wi_dust_64x64},
    {.id = 741, .ico = wi_fog_64x64},
    {.id = 751, .ico = wi_sandstorm_64x64},
    {.id = 761, .ico = wi_dust_64x64},
    {.id = 762, .ico = wi_volcano_64x64},
    {.id = 771, .ico = wi_strong_wind_64x64},
    {.id = 781, .ico = wi_tornado_64x64},

    // Group 800: Clear
    {.id = 800, .ico = wi_horizon_alt_64x64},
    // Group 80x: Clouds
    {.id = 801, .ico = wi_day_cloudy_high_64x64},
    {.id = 802, .ico = wi_day_cloudy_64x64},
    {.id = 803, .ico = wi_cloud_64x64},
    {.id = 804, .ico = wi_cloudy_64x64},
};

IconsKeyValuePair night_icons_harshtable[HARSH_MAX] = {
    // Group 2xx: Thunderstorm
    {.id = 200, .ico = wi_day_snow_thunderstorm_64x64},
    {.id = 201, .ico = wi_day_thunderstorm_64x64},
    {.id = 202, .ico = wi_day_thunderstorm_64x64},

    {.id = 210, .ico = wi_day_lightning_64x64},
    {.id = 211, .ico = wi_lightning_64x64},
    {.id = 212, .ico = wi_lightning_64x64},
    {.id = 221, .ico = wi_lightning_64x64},

    {.id = 230, .ico = wi_day_sleet_storm_64x64},
    {.id = 231, .ico = wi_day_sleet_storm_64x64},
    {.id = 232, .ico = wi_day_sleet_storm_64x64},

    // Group 3xx: Drizzle
    {.id = 300, .ico = wi_day_snow_wind_64x64},
    {.id = 301, .ico = wi_day_sleet_64x64},
    {.id = 302, .ico = wi_day_sprinkle_64x64},

    {.id = 310, .ico = wi_day_sprinkle_64x64},
    {.id = 311, .ico = wi_day_sprinkle_64x64},
    {.id = 312, .ico = wi_day_sprinkle_64x64},

    {.id = 313, .ico = wi_day_sprinkle_64x64},
    {.id = 314, .ico = wi_day_sprinkle_64x64},
    {.id = 321, .ico = wi_day_sprinkle_64x64},

    // Group 5xx: Rain
    {.id = 500, .ico = wi_day_showers_64x64},
    {.id = 501, .ico = wi_day_rain_mix_64x64},
    {.id = 502, .ico = wi_day_rain_64x64},
    {.id = 503, .ico = wi_rain_64x64},
    {.id = 504, .ico = wi_rain_64x64},

    {.id = 511, .ico = wi_snowflake_cold_64x64},

    {.id = 520, .ico = wi_day_sprinkle_64x64},
    {.id = 521, .ico = wi_day_sprinkle_64x64},
    {.id = 522, .ico = wi_day_sprinkle_64x64},
    {.id = 531, .ico = wi_day_sprinkle_64x64},

    // Group 7xx: Atmosphere
    {.id = 701, .ico = wi_fog_64x64},
    {.id = 711, .ico = wi_smoke_64x64},
    {.id = 721, .ico = wi_smoke_64x64},

    {.id = 731, .ico = wi_dust_64x64},
    {.id = 741, .ico = wi_fog_64x64},
    {.id = 751, .ico = wi_sandstorm_64x64},
    {.id = 761, .ico = wi_dust_64x64},
    {.id = 762, .ico = wi_volcano_64x64},
    {.id = 771, .ico = wi_strong_wind_64x64},
    {.id = 781, .ico = wi_tornado_64x64},

    // Group 800: Clear
    {.id = 800, .ico = wi_horizon_alt_64x64},
    // Group 80x: Clouds
    {.id = 801, .ico = wi_day_cloudy_high_64x64},
    {.id = 802, .ico = wi_day_cloudy_64x64},
    {.id = 803, .ico = wi_cloud_64x64},
    {.id = 804, .ico = wi_cloudy_64x64},
};

static inline unsigned int Harsh_HashFunction(int id)
{
    return id % HARSH_MAX;
}

const unsigned char *Harsh_GetAddressById(int id)
{
    unsigned int index = Harsh_HashFunction(id);

    while (index < HARSH_MAX)
    {
        if (day_icons_harshtable[index].id == id)
            return day_icons_harshtable[index].ico;
        index = (index + 1) % HARSH_MAX;
    }

    return NULL; // ID not found
}

#define SPACE_BETWEEN_IMAGES (int)15
#define FORECAST_ICONS_Y_AXIS (int)190
void Renderer_RenderWeeklyForecast(EpdDevice_t *display, struct WeatherapiDescDailyWeather_t *forecast)
{
    static char txtbff[10] = {0};
    int xaxis, yaxis, dayofweek;

    for (int i = 0; i < 5; i++)
    {
        xaxis = 10 + (i * SPACE_BETWEEN_IMAGES) + (i * 64);
        yaxis = FORECAST_ICONS_Y_AXIS;
        dayofweek = Conv_Epoch2DayOfWeek(&forecast[i].dt);
        Conv_DayOfWeek2String(dayofweek, txtbff);
        const unsigned char *icon = Harsh_GetAddressById(forecast[i].weather[0].id);

        if (icon == NULL)
            icon = wi_gale_warning_64x64;

        Epd_DisplayBmp(display, xaxis, yaxis, 64, 64, icon, false);         // Weather icon
        Epd_DrawText(display, xaxis + 6, (yaxis += 64), txtbff, 12, false); // Day of week

        // Temps
        Epd_DisplayBmp(display, xaxis - 10, (yaxis += 16) + 2, 24, 24, wi_thermometer_24x24, false);
        sprintf(txtbff, "%dC", (int)Conv_Kelvin2Celcius(forecast[i].temp.max));
        Epd_DrawText(display, xaxis + 12, yaxis, txtbff, 12, false);
        sprintf(txtbff, "%dC", (int)Conv_Kelvin2Celcius(forecast[i].temp.min));
        Epd_DrawText(display, xaxis + 12, (yaxis += 13), txtbff, 12, false);
    }
}

void Renderer_Render(EpdDevice_t *display, WeatherapiResponse_t *weather)
{
    memset(display->buffer, 0xff, EPD_BUFFER_SIZE);
    if (weather == NULL)
    {
        Epd_DisplayBmp(display, 168, 100, 64, 64, error_icon_64x64, false);
        Epd_DrawText(display, 5, 170, "Houve um erro ao buscar os dados", 12, false);
        goto render;
    }

    Renderer_RenderWeeklyForecast(display, weather->daily);

render:
    Epd_Render(display);
}

void Renderer_RenderLoading(EpdDevice_t *display)
{
    memset(display->buffer, 0xff, EPD_BUFFER_SIZE);
    Epd_DisplayBmp(display, 168, 100, 64, 64, wi_cloud_refresh_64x64, false);
    Epd_DrawText(display, 100, 170, "Atualizando dados", 12, false);
    Epd_Render(display);
}

void Renderer_RenderWifiError(EpdDevice_t *display)
{
    memset(display->buffer, 0xff, EPD_BUFFER_SIZE);
    Epd_DisplayBmp(display, 168, 100, 64, 64, wifi_off_64x64, false);
    Epd_DrawText(display, 125, 170, "Erro de rede", 12, false);
    Epd_Render(display);
}
