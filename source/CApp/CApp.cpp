#include <SDL_events.h>
#include "../../include/CApp.hpp"


CApp* CApp::_SpCAppInstance = nullptr;


CApp* CApp::getInstance()
{
    if (_SpCAppInstance == nullptr) _SpCAppInstance = new CApp();
    return _SpCAppInstance;
}


CApp::CApp() noexcept : CEvent{}, _bRunning{true},  _pSdlSurfaceDisplay{nullptr}, 
    _pSdlSurfaceGrid{nullptr}, _pSdlSurfaceX{nullptr}, _pSdlSurfaceO{nullptr}, _grid{}, 
    _EPlayerMarkCurrent{Grid::PlayerMark::GRID_TYPE_O} {}


void CApp::OnExecute()
{
    OnInit();

    SDL_Event sdlEvent;
 
    while(_bRunning) 
    {
        while(SDL_PollEvent(&sdlEvent)) OnEvent(&sdlEvent);
 
        OnLoop();
        OnRender();
    }
 
    OnCleanup();
}


void CApp::Reset() noexcept
{
    _grid = Grid{};
}
