#include <SDL_video.h>
#include <SDL_stdinc.h>
#include "../../include/CApp.hpp"
#include "../../include/CSurface.hpp"


void CApp::OnRender() 
{
    CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceGrid, 0, 0);

    for(Sint32 i = 0; i < _grid.getHeight(); i++) 
    {
        Sint32 iY = i * (_pSdlSurfaceDisplay->h / _grid.getHeight());

        for (Sint32 j = 0; j < _grid.getWidth(); j++)
        {
            Sint32 iX = j * (_pSdlSurfaceDisplay->w / _grid.getWidth());

            if(_grid[i][j] == Grid::PlayerMark::GRID_TYPE_X) 
                CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceX, iX, iY);
            else if(_grid[i][j] == Grid::PlayerMark::GRID_TYPE_O) 
                CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceO, iX, iY);
        }
    }
 
    SDL_Flip(_pSdlSurfaceDisplay);  // Refreshes the screen
}
