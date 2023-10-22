#include "conversion.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

inline float Conv_Kelvin2Celcius(float v)
{
    if (!v)
        return 0;
    return v - 273.15;
}

void Conv_Epoch2TimeString(const unsigned long *epoch_time, char bff[6])
{
    int hours = 0, minutes = 0;

    hours = (*epoch_time / 3600) % 24;
    minutes = (*epoch_time % 3600) / 60;

    sprintf(bff, hours < 10 ? "0%d:" : "%d:", hours + TIMEZONE_OFFSET);
    sprintf(bff + 3, minutes < 10 ? "0%d" : "%d", minutes);
}

int Conv_Epoch2DayOfWeek(const unsigned long *epoch_time)
{
    static const unsigned long sec_per_day = 86400; // 24 hours * 60 minutes * 60 seconds
    return ((*epoch_time / sec_per_day) + 3) % 7;   // Unix reference day (Thursday)
}

void Conv_DayOfWeek2String(unsigned int dayofweek, char buff[4])
{
    static const char *week[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
    if (dayofweek > 7 || dayofweek < 0)
        return;
    memcpy(buff, week[dayofweek], 4);
}
