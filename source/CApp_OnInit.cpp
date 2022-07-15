#include <string>
#include <stdexcept>
#include <ios>

#ifdef HW_RVL
	#include <fat.h>
#endif

#include "../include/CApp.hpp"
#include "../include/CSurface.hpp"


//SDL_Joystick* joysticks[JOYNUMS];


void CApp::OnInit()
{
    #ifdef HW_RVL
		fatInitDefault();
	#endif

    if(SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_CDROM) < 0) 
        throw std::runtime_error(std::string("Unable to init SDL: ") + SDL_GetError());

    if ((__pSdlSurfaceDisplay = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16, 
        SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN)) == nullptr) 
        throw std::runtime_error(std::string("Unable to set video: ") + SDL_GetError());

    try { __pSdlSurfaceTest = CSurface::OnLoad("sd:/apps/test/resources/myimage.bmp"); }
    catch (const std::ios_base::failure& iof) { throw; }
	
	/*for(u8 i = 0; i < JOYNUMS; i++) joysticks[i] = SDL_JoystickOpen(i);		//Open each joystick

	SDL_WM_SetCaption("Joytest", nullptr);
    SDL_ShowCursor(SDL_DISABLE);
 
    // Make sure SDL cleans up before exit
    atexit(Cleanup);*/
}
