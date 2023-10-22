#include "renderer.h"
#include "conversion.h"
#include <stdio.h>
#include <string.h>

#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/TomThumb.h>
#include <GxEPD.h>
#include <GxGDEW042T2/GxGDEW042T2.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include "icons/icons_24x24.h"
#include "icons/icons_32x32.h"
#include "icons/icons_64x64.h"
#include "icons/icons_76x76.h"

#define HARSH_MAX 44

typedef struct
{
    int id;
    unsigned const char *ico;
} IconsKeyValuePair;

IconsKeyValuePair day_icons_harshtable64[HARSH_MAX] = {
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
    {.id = 800, .ico = wi_day_sunny_64x64},
    // Group 80x: Clouds
    {.id = 801, .ico = wi_day_cloudy_high_64x64},
    {.id = 802, .ico = wi_day_cloudy_64x64},
    {.id = 803, .ico = wi_cloud_64x64},
    {.id = 804, .ico = wi_cloudy_64x64},
};
IconsKeyValuePair night_icons_harshtable64[HARSH_MAX] = {
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

IconsKeyValuePair day_icons_harshtable76[HARSH_MAX] = {
    // Group 2xx: Thunderstorm
    {.id = 200, .ico = wi_day_snow_thunderstorm_76x76},
    {.id = 201, .ico = wi_day_thunderstorm_76x76},
    {.id = 202, .ico = wi_day_thunderstorm_76x76},

    {.id = 210, .ico = wi_day_lightning_76x76},
    {.id = 211, .ico = wi_lightning_76x76},
    {.id = 212, .ico = wi_lightning_76x76},
    {.id = 221, .ico = wi_lightning_76x76},

    {.id = 230, .ico = wi_day_sleet_storm_76x76},
    {.id = 231, .ico = wi_day_sleet_storm_76x76},
    {.id = 232, .ico = wi_day_sleet_storm_76x76},

    // Group 3xx: Drizzle
    {.id = 300, .ico = wi_day_snow_wind_76x76},
    {.id = 301, .ico = wi_day_sleet_76x76},
    {.id = 302, .ico = wi_day_sprinkle_76x76},

    {.id = 310, .ico = wi_day_sprinkle_76x76},
    {.id = 311, .ico = wi_day_sprinkle_76x76},
    {.id = 312, .ico = wi_day_sprinkle_76x76},

    {.id = 313, .ico = wi_day_sprinkle_76x76},
    {.id = 314, .ico = wi_day_sprinkle_76x76},
    {.id = 321, .ico = wi_day_sprinkle_76x76},

    // Group 5xx: Rain
    {.id = 500, .ico = wi_day_showers_76x76},
    {.id = 501, .ico = wi_day_rain_mix_76x76},
    {.id = 502, .ico = wi_day_rain_76x76},
    {.id = 503, .ico = wi_rain_76x76},
    {.id = 504, .ico = wi_rain_76x76},

    {.id = 511, .ico = wi_snowflake_cold_76x76},

    {.id = 520, .ico = wi_day_sprinkle_76x76},
    {.id = 521, .ico = wi_day_sprinkle_76x76},
    {.id = 522, .ico = wi_day_sprinkle_76x76},
    {.id = 531, .ico = wi_day_sprinkle_76x76},

    // Group 7xx: Atmosphere
    {.id = 701, .ico = wi_fog_76x76},
    {.id = 711, .ico = wi_smoke_76x76},
    {.id = 721, .ico = wi_smoke_76x76},

    {.id = 731, .ico = wi_dust_76x76},
    {.id = 741, .ico = wi_fog_76x76},
    {.id = 751, .ico = wi_sandstorm_76x76},
    {.id = 761, .ico = wi_dust_76x76},
    {.id = 762, .ico = wi_volcano_76x76},
    {.id = 771, .ico = wi_strong_wind_76x76},
    {.id = 781, .ico = wi_tornado_76x76},

    // Group 800: Clear
    {.id = 800, .ico = wi_day_sunny_76x76},
    // Group 80x: Clouds
    {.id = 801, .ico = wi_day_cloudy_high_76x76},
    {.id = 802, .ico = wi_day_cloudy_76x76},
    {.id = 803, .ico = wi_cloud_76x76},
    {.id = 804, .ico = wi_cloudy_76x76},
};

const unsigned char *Harsh_GetAddressById(int id, int size, char type)
{
    unsigned int index = id % HARSH_MAX;

    IconsKeyValuePair *table = NULL;

    if (type == 'd')
    {
        switch (size)
        {
        case 24:
            table = day_icons_harshtable64;
            break;
        case 32:
            table = day_icons_harshtable64;
            break;
        case 64:
            table = night_icons_harshtable64;
            break;
        case 76:
            table = day_icons_harshtable76;
            break;
        default:
            break;
        }
    }

    else if (type == 'n')
    {
        switch (size)
        {
        case 24:
            table = day_icons_harshtable64;
            break;
        case 32:
            table = day_icons_harshtable64;
            break;
        case 64:
            table = day_icons_harshtable64;
            break;
        case 76:
            table = day_icons_harshtable64;
            break;
        default:
            break;
        }
    }

    if (size == 64 && type == 'n')
        table = night_icons_harshtable64;

    if (table == NULL)
        goto err;

    while (index < HARSH_MAX)
    {
        if (table[index].id == id)
            return table[index].ico;
        index = (index + 1) % HARSH_MAX;
    }

err:
    return NULL; // ID not found
}

void Renderer_RenderWeeklyForecast(GxEPD_Class *display, struct WeatherapiDescDailyWeather_t *forecast)
{
    static char dayofweek[10] = {0};
    int xaxis, yaxis;

    for (int i = 0; i < 5; i++)
    {
        xaxis = 10 + (i * 15) + (i * 64);
        yaxis = 190;
        Conv_DayOfWeek2String(Conv_Epoch2DayOfWeek(&forecast[i].dt), dayofweek);
        const unsigned char *icon = Harsh_GetAddressById(forecast[i].weather[0].id, 64, 'd');

        if (icon == NULL)
            icon = wi_gale_warning_64x64;

        display->drawBitmap(icon, xaxis, yaxis, 64, 64, GxEPD_BLACK, GxEPD::bm_mode::bm_invert);
        display->setTextColor(GxEPD_BLACK);
        display->setCursor(xaxis + 8, (yaxis += 76));
        display->setFont(&FreeMono12pt7b);
        display->print(dayofweek);

        display->drawBitmap(wi_thermometer_24x24, xaxis - 8, yaxis + 8, 24, 24, GxEPD_BLACK, GxEPD::bm_mode::bm_invert);

        display->setFont(&FreeMono9pt7b);
        display->setCursor(xaxis + 12, (yaxis += 16));
        display->printf("%dC", (int)Conv_Kelvin2Celcius(forecast[i].temp.max));

        display->setCursor(xaxis + 12, (yaxis += 13));
        display->printf("%dC", (int)Conv_Kelvin2Celcius(forecast[i].temp.min));
    }
}

#define INFO_SECTION_SIZE (int)126
static void Renderer_DrawInfo(GxEPD_Class *display, const uint8_t *bitmap, int x, int y, const char *label,
                              const char *info)
{
    display->drawBitmap(bitmap, x, (y += 26), 32, 32, GxEPD_BLACK, GxEPD::bm_mode::bm_invert);
    display->setCursor((x + 36), (y += 8));
    display->print(label);
    display->setCursor((x + 36), y + 14);
    display->print(info);
}
void Renderer_RenderCurrentWeather(GxEPD_Class *display, struct WeatherapiDescCurrentWeather_t *weather)
{
    int xaxis = 10, yaxis = 5;
    const unsigned char *icon = Harsh_GetAddressById(weather->weather[0].id, 76, 'd');
    if (icon == NULL)
        icon = wi_gale_warning_76x76;

    display->drawBitmap(icon, xaxis, yaxis, 76, 76, GxEPD_BLACK, GxEPD::bm_mode::bm_invert);

    display->setFont(&FreeMono24pt7b);
    display->setCursor((xaxis + 76), (yaxis += 36));
    display->printf("%dC", (int)Conv_Kelvin2Celcius(weather->temp));
    display->setFont(&FreeMono9pt7b);
    display->setCursor((xaxis + 76), (yaxis += 16));
    display->printf("%dC|%dC", (int)Conv_Kelvin2Celcius(weather->tmax), (int)Conv_Kelvin2Celcius(weather->tmin));

    char buff[12] = {'\n'};

    Conv_Epoch2TimeString(&weather->sunrise, buff);
    Renderer_DrawInfo(display, wi_sunrise_32x32, xaxis, yaxis, "Sunrise", buff);
    Conv_Epoch2TimeString(&weather->sunset, buff);
    Renderer_DrawInfo(display, wi_sunset_32x32, (xaxis += INFO_SECTION_SIZE), yaxis, "Sunset", buff);

    // sprintf(buff, "%s%dKm", weather->visibility >= 10000 ? "<" : "", weather->visibility / 1000);
    // Renderer_DrawInfo(display, visibility_icon_32x32, (xaxis += INFO_SECTION_SIZE), yaxis, "Distance", buff);

    sprintf(buff, "%d%%", weather->clouds);
    Renderer_DrawInfo(display, wi_cloud_32x32, (xaxis += INFO_SECTION_SIZE), yaxis, "Clouds", buff);

    sprintf(buff, "%.2f", weather->uvi);
    Renderer_DrawInfo(display, wi_day_sunny_32x32, (xaxis = 10), (yaxis += 56), "UV Index", buff);

    sprintf(buff, "%dhPa", weather->pressure);
    Renderer_DrawInfo(display, wi_barometer_32x32, (xaxis += INFO_SECTION_SIZE), yaxis, "Pressure", buff);

    sprintf(buff, "%d%%", weather->humidity);
    Renderer_DrawInfo(display, wi_humidity_32x32, (xaxis += INFO_SECTION_SIZE), yaxis, "Humidity", buff);
}

static void Renderer_DrawMessage(GxEPD_Class *display, const char *message, const uint8_t *bitmap)
{
    display->fillScreen(GxEPD_WHITE);
    display->drawBitmap(bitmap, 168, 100, 64, 64, GxEPD_BLACK, GxEPD::bm_mode::bm_invert);
    display->setTextColor(GxEPD_BLACK);
    display->setFont(&FreeMono9pt7b);

    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display->getTextBounds(message, 0, 0, &tbx, &tby, &tbw, &tbh);

    display->setCursor(((display->width() - tbw) / 2) - tbx, 200);
    display->print(message);
    display->update();
}

void Renderer_RenderLoading(GxEPD_Class *display)
{
    const char *message = "Atualizando dados";
    Renderer_DrawMessage(display, message, wi_cloud_refresh_64x64);
}

void Renderer_RenderWifiError(GxEPD_Class *display)
{
    const char *message = "Erro de rede";
    Renderer_DrawMessage(display, message, wifi_off_64x64);
}

void Renderer_Render(GxEPD_Class *display, WeatherapiResponse_t *weather)
{
    if (weather == NULL)
    {
        const char *message = "Houve um erro ao buscar os dados";
        Renderer_DrawMessage(display, message, error_icon_64x64);
    }

    display->fillScreen(GxEPD_WHITE);

    Renderer_RenderWeeklyForecast(display, weather->daily);
    Renderer_RenderCurrentWeather(display, &weather->current);

    display->update();
}
