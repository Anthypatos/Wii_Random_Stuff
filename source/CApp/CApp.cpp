#include <SDL_events.h>
#include <SDL_stdinc.h>
#include "../../include/CApp.hpp"


CApp* CApp::_SpCAppInstance = nullptr;


CApp* CApp::getInstance()
{
    if (_SpCAppInstance == nullptr) _SpCAppInstance = new CApp();
    return _SpCAppInstance;
}


CApp::CApp() noexcept : CEvent{}, _bRunning{true},  _pSdlSurfaceDisplay{nullptr}, 
    _pSdlSurfaceGrid{nullptr}, _pSdlSurfaceX{nullptr}, _pSdlSurfaceO{nullptr}, _grid{3, 3}, 
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
    _grid = Grid{_grid.getHeight(), _grid.getWidth()};
}


void CApp::OnInputFocus() {}
void CApp::OnInputBlur() {}
void CApp::OnKeyDown(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode) {}
void CApp::OnKeyUp(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode) {}
void CApp::OnMouseFocus() {}
void CApp::OnMouseBlur() {}
void CApp::OnMouseMove(Sint32 iMouseX, Sint32 iMouseY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
    bool bRight, bool bMiddle) {}
void CApp::OnMouseWheel(bool bUp, bool bDown) {}


void CApp::OnLButtonDown(Sint32 iMouseX, Sint32 iMouseY) 
{
    Sint32 iRow = iMouseY / (_pSdlSurfaceDisplay->h / _grid.getHeight());
    Sint32 iColumn = iMouseX / (_pSdlSurfaceDisplay->w / _grid.getWidth());
 
    if (_grid.isValidPlay(iRow, iColumn))
    {
        _grid.makePlay(_EPlayerMarkCurrent, iRow, iColumn);
        _EPlayerMarkCurrent = Grid::nextPlayer(_EPlayerMarkCurrent);
    }
}


void CApp::OnLButtonUp(Sint32 iMouseX, Sint32 iMouseY) {}
void CApp::OnRButtonDown(Sint32 iMouseX, Sint32 iMouseY) {}
void CApp::OnRButtonUp(Sint32 iMouseX, Sint32 iMouseY) {}
void CApp::OnMButtonDown(Sint32 iMouseX, Sint32 iMouseY) {}
void CApp::OnMButtonUp(Sint32 iMouseX, Sint32 iMouseY) {}
void CApp::OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue) {}
void CApp::OnJoyButtonDown(Uint8 yWhich, Uint8 yButton) {}
void CApp::OnJoyButtonUp(Uint8 yWhich, Uint8 yButton) {}
void CApp::OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue) {}
void CApp::OnJoyBall(Uint8 yWhich, Uint8 yBall, Sint16 rRelX, Sint16 rRelY) {}
void CApp::OnMinimize() {}
void CApp::OnRestore() {}
void CApp::OnResize(Sint32 iWidth, Sint32 iHeight) {}
void CApp::OnExpose() {}
void CApp::OnUser(Uint8 yType, Sint32 iCode, void* pData1, void* pData2) {}
