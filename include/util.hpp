#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <ogc/gx_struct.h>
#include <wiiuse/wpad.h>

#include <SDL_video.h>
#include <SDL_joystick.h>


#define JOYNUMS 4


void print_wiimote_data(void* pXfb, const GXRModeObj* CpGXRmode, WPADData* pWPADData);
void state_in_game(SDL_Surface* pSDLSurfaceScreen, SDL_Joystick** joysticks);


#endif