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


Uint8 CApp::SyJoysticks = 4;
//SDL_Joystick* joysticks[JOYNUMS];


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

    SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);

    try 
    { 
        _pSdlSurfaceGrid = CSurface::OnLoad("/apps/test/resources/gfx/grid.bmp");
        _pSdlSurfaceX = CSurface::OnLoad("/apps/test/resources/gfx/red.bmp");
        _pSdlSurfaceO = CSurface::OnLoad("/apps/test/resources/gfx/yellow.bmp");
    }
    catch (const std::ios_base::failure& Ciof) { throw; }

    CSurface::Transparent(_pSdlSurfaceX, 255, 0, 255);
    CSurface::Transparent(_pSdlSurfaceO, 255, 0, 255);
	
	/*for(uint8_t i = 0; i < JOYNUMS; i++) joysticks[i] = SDL_JoystickOpen(i);		//Open each joystick

	SDL_WM_SetCaption("Joytest", nullptr);
    SDL_ShowCursor(SDL_DISABLE);
 
    // Make sure SDL cleans up before exit
    atexit(Cleanup);*/
}
