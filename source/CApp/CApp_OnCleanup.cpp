#include <SDL.h>
#include <SDL_video.h>
#include "../../include/CApp.hpp"


void CApp::OnCleanup() const noexcept 
{
    SDL_FreeSurface(_pSdlSurfaceStart);
	SDL_FreeSurface(_pSdlSurfaceGrid);
    SDL_FreeSurface(_pSdlSurfaceRed);
    SDL_FreeSurface(_pSdlSurfaceYellow);
    SDL_FreeSurface(_pSdlSurfaceWinRed);
    SDL_FreeSurface(_pSdlSurfaceWinYellow);
    SDL_Quit(); 

    /*for(uint8_t i = 0; i < JOYNUMS; i++) SDL_JoystickClose(joysticks[i]);	//Close each joysticks*/
}
