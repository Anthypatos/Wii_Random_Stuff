#include <iostream>
#include <cstdlib>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#ifdef HW_RVL
	#include <ogc/gx_struct.h>
	#include <ogc/video_types.h>
	#include <ogc/consol.h>
	#include <fat.h>
#endif

#include "../include/CApp.hpp"


int main(int argc, char** argv)
{
	CApp* pCApp = CApp::getInstance();
	pCApp->OnExecute();
	
	/*while (true)
    {
		#ifdef HW_RVL
			CON_Init(theApp._pSdlSurfaceDisplay->pixels, 20, 20, theApp._pSdlSurfaceDisplay->w, 
				theApp._pSdlSurfaceDisplay->h, theApp._pSdlSurfaceDisplay->w * VI_DISPLAY_PIX_SZ);
			std::cout << "\x1b[2;0H";
			SDL_FillRect(theApp._pSdlSurfaceDisplay, nullptr, 
				SDL_MapRGB(theApp._pSdlSurfaceDisplay->format, 0, 0, 0));
		#endif

		//state_in_game();
    }*/

	return 0;
}
