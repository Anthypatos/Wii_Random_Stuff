#include <SDL_events.h>
#include "../../include/CApp.hpp"


CApp* CApp::_SpCAppInstance = nullptr;


CApp* CApp::getInstance()
{
    if (_SpCAppInstance == nullptr) _SpCAppInstance = new CApp();
    return _SpCAppInstance;
}


CApp::CApp() noexcept : CEvent{}, _bRunning{true},  _pSdlSurfaceDisplay{nullptr}, 
    _pSdlSurfaceStart{nullptr}, _pSdlSurfaceGrid{nullptr}, _pSdlSurfaceRed{nullptr}, 
    _pSdlSurfaceYellow{nullptr}, _pSdlSurfaceWinRed{nullptr}, _pSdlSurfaceWinYellow{nullptr}, _grid{}, 
    _EplayerMarkCurrent{Grid::PlayerMark::GRID_TYPE_RED}, _ECurrentState{State_t::STATE_START} {}


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
    _ECurrentState = STATE_START;
    _grid = Grid{};
    _EplayerMarkCurrent = Grid::PlayerMark::GRID_TYPE_RED;
}
