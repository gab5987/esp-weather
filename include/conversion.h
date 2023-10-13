#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <esp_system.h>

float kelvin_to_celcius(float v);
void epoch_to_time_string(const unsigned long *epoch_time, char bff[6]);

#endif
