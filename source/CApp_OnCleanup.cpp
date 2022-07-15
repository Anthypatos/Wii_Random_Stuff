#include <SDL.h>
#include <SDL_video.h>
#include "../include/CApp.hpp"


void CApp::OnCleanup() const noexcept 
{
	SDL_FreeSurface(__pSdlSurfaceTest);
    SDL_Quit(); 

    /*for(u8 i = 0; i < JOYNUMS; i++) SDL_JoystickClose(joysticks[i]);	//Close each joysticks*/
}
