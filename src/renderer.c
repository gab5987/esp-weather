#include "renderer.h"
#include "icons/icons_64x64.h"

void Renderer_RenderWeeklyForecast(EpdDevice_t *display, struct WeatherapiDescDailyWeather_t *forecast)
{
    Epd_DisplayBmp(display, 10, 200, 64, 64, wi_cloud_64x64, false);
    Epd_DisplayBmp(display, 70, 200, 64, 64, wi_cloudy_gusts_64x64, false);
    Epd_DisplayBmp(display, 130, 200, 64, 64, wi_day_sunny_overcast_64x64, false);
    Epd_DisplayBmp(display, 200, 200, 64, 64, wi_raindrops_64x64, false);
    Epd_DisplayBmp(display, 270, 200, 64, 64, wi_night_snow_thunderstorm_64x64, false);
    Epd_Render(display);
}

void Renderer_Render(EpdDevice_t *display, WeatherapiResponse_t *weather)
{
    Renderer_RenderWeeklyForecast(display, weather->daily);
}
