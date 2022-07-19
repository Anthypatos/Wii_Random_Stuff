#include <SDL.h>
#include <SDL_video.h>
#include "../../include/CApp.hpp"


void CApp::OnCleanup() const noexcept 
{
	SDL_FreeSurface(_pSdlSurfaceGrid);
    SDL_FreeSurface(_pSdlSurfaceX);
    SDL_FreeSurface(_pSdlSurfaceO);
    SDL_Quit(); 

    /*for(uint8_t i = 0; i < JOYNUMS; i++) SDL_JoystickClose(joysticks[i]);	//Close each joysticks*/
}
