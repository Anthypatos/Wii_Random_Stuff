#include <SDL.h>
#include <SDL_image.h>

#include "../include/CApp.hpp"


int main(int argc, char** argv)
{
	CApp* pCApp = CApp::getInstance();
	pCApp->OnExecute();
	
	/*while (true)
    {
		//state_in_game();
    }*/

	return 0;
}
