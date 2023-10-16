#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "epd4i2.h"
#include "weatherapi_response.h"

void Renderer_Render(EpdDevice_t *display, WeatherapiResponse_t *weather);

#endif // !_RENDERER_H_
