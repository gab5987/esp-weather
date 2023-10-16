#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <esp_system.h>

float Conv_Kelvin2Celcius(float v);
void Conv_Epoch2TimeString(const unsigned long *epoch_time, char bff[6]);

#endif
