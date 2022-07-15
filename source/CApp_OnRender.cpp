#include "../include/CApp.hpp"
#include "../include/CSurface.hpp"


void CApp::OnRender() const noexcept 
{
    CSurface::OnDraw(__pSdlSurfaceDisplay, __pSdlSurfaceTest, 0, 0);
    CSurface::OnDraw(__pSdlSurfaceDisplay, __pSdlSurfaceTest, 100, 100, 0, 0, 50, 50);
 
    SDL_Flip(__pSdlSurfaceDisplay);  // Refreshes the screen
}
