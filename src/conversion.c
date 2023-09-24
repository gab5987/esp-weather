#include "core/conversion.h"
#include "config.h"
#include <stdio.h>

inline float kelvin_to_celcius(float v)
{
    if (!v)
        return 0;
    return v - 273.15;
}

void epoch_to_time_string(const unsigned long *epoch_time, char bff[6])
{
    int hours = 0, minutes = 0;

    hours = (*epoch_time / 3600) % 24;
    minutes = (*epoch_time % 3600) / 60;

    sprintf(bff, hours < 10 ? "0%d:" : "%d:", hours + TIMEZONE_OFFSET);
    sprintf(bff + 3, minutes < 10 ? "0%d" : "%d", minutes);
}
