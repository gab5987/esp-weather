#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <esp_system.h>

#ifdef __cplusplus
extern "C"
{
#endif
    float Conv_Kelvin2Celcius(float v);
    void Conv_Epoch2TimeString(const unsigned long *epoch_time, char bff[6]);
    int Conv_Epoch2DayOfWeek(const unsigned long *epoch_time);
    void Conv_DayOfWeek2String(unsigned int dayofweek, char buff[4]);
#ifdef __cplusplus
}
#endif

#endif
