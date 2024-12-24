#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <ogc/gx_struct.h>
#include <wiiuse/wpad.h>

#include <SDL_video.h>
#include <SDL_joystick.h>


void PrintWiimoteData(void* pXfb, const GXRModeObj* CpGXRmode, WPADData* pWPADData);
void StateInGame(SDL_Surface* pSDLSurfaceScreen, SDL_Joystick** joysticks);


#endif