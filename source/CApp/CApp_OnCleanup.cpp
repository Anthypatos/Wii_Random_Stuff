#include <SDL.h>
#include <SDL_video.h>
#include "../../include/CApp.hpp"


void CApp::OnCleanup() noexcept 
{
    SDL_FreeSurface(_pSdlSurfaceStart);
	SDL_FreeSurface(_pSdlSurfaceGrid);
    SDL_FreeSurface(_pSdlSurfaceRed);
    SDL_FreeSurface(_pSdlSurfaceYellow);
    SDL_FreeSurface(_pSdlSurfaceWinRed);
    SDL_FreeSurface(_pSdlSurfaceWinYellow);
    
    for (std::vector<Player*>::iterator i = _apPlayer.begin(); i != _apPlayer.end(); ++i) delete *i;

    SDL_Quit();
}
