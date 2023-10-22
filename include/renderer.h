#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "weatherapi_response.h"

#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD.h>
#include <GxGDEW042T2/GxGDEW042T2.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

void Renderer_RenderLoading(GxEPD_Class *display);
void Renderer_RenderWifiError(GxEPD_Class *display);

void Renderer_Render(GxEPD_Class *display, WeatherapiResponse_t *weather);

#endif // !_RENDERER_H_
