#ifndef _WIFI_H_
#define _WIFI_H_

#include <esp_err.h>
#include <esp_system.h>
bool Wifi_Initialize(const char *ssid, const char *password);

#endif /* _WIFI_H_ */

