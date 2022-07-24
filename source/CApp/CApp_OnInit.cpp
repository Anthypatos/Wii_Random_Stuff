#include <string>
#include <stdexcept>
#include <ios>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_stdinc.h>

#ifdef HW_RVL
	#include <fat.h>
#endif

#include "../../include/CApp.hpp"
#include "../../include/CSurface.hpp"


void CApp::OnInit()
{
    Uint32 iInitFlags = SDL_INIT_EVERYTHING;

    #ifdef HW_RVL
		fatInitDefault();
        iInitFlags &= ~SDL_INIT_CDROM;
	#endif

    if(SDL_Init(iInitFlags) < 0) 
        throw std::runtime_error(std::string("Unable to init SDL: ") + SDL_GetError());

    if ((_pSdlSurfaceDisplay = SDL_SetVideoMode(CApp::SCrWindowWidth, CApp::SCrWindowHeight, 16, 
        SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN)) == nullptr) 
        throw std::runtime_error(std::string("Unable to set video: ") + SDL_GetError());

    _apPlayer = new Player(Grid::PlayerMark::GRID_TYPE_RED);
    SDL_JoystickEventState(SDL_ENABLE);

    try 
    { 
        _pSdlSurfaceStart = CSurface::OnLoad("/apps/test/resources/gfx/start.bmp");
        _pSdlSurfaceGrid = CSurface::OnLoad("/apps/test/resources/gfx/grid.bmp");
        _pSdlSurfaceRed = CSurface::OnLoad("/apps/test/resources/gfx/red.bmp");
        _pSdlSurfaceYellow = CSurface::OnLoad("/apps/test/resources/gfx/yellow.bmp");
        _pSdlSurfaceWinRed = CSurface::OnLoad("/apps/test/resources/gfx/winRed.bmp");
        _pSdlSurfaceWinYellow = CSurface::OnLoad("/apps/test/resources/gfx/winYellow.bmp");
    }
    catch (const std::ios_base::failure& Ciof) { throw; }

    CSurface::Transparent(_pSdlSurfaceRed, 255, 0, 255);
    CSurface::Transparent(_pSdlSurfaceYellow, 255, 0, 255);

	/*SDL_WM_SetCaption("Joytest", nullptr);
    SDL_ShowCursor(SDL_DISABLE);
 
    // Make sure SDL cleans up before exit
    atexit(Cleanup);*/
}
