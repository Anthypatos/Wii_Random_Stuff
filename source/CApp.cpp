#include <SDL_events.h>
#include "../include/CApp.hpp"


CApp* CApp::_SpCAppInstance = nullptr;


CApp* CApp::getInstance()
{
    if (_SpCAppInstance == nullptr) _SpCAppInstance = new CApp();
    return _SpCAppInstance;
}


CApp::CApp() noexcept : __bRunning{true},  __pSdlSurfaceDisplay{nullptr}, __pSdlSurfaceTest{nullptr} {}


void CApp::OnExecute()
{
    OnInit();

    SDL_Event sdlEvent;
 
    while(__bRunning) 
    {
        while(SDL_PollEvent(&sdlEvent)) OnEvent(&sdlEvent);
 
        OnLoop();
        OnRender();
    }
 
    OnCleanup();
}


void CApp::OnInputFocus() {}
void CApp::OnInputBlur() {}
void CApp::OnKeyDown(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode) {}
void CApp::OnKeyUp(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode) {}
void CApp::OnMouseFocus() {}
void CApp::OnMouseBlur() {}
void CApp::OnMouseMove(Sint32 iMX, Sint32 iMY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
    bool bRight, bool bMiddle) {}
void CApp::OnMouseWheel(bool bUp, bool bDown) {}
void CApp::OnLButtonDown(Sint32 iMX, Sint32 iMY) {}
void CApp::OnLButtonUp(Sint32 iMX, Sint32 iMY) {}
void CApp::OnRButtonDown(Sint32 iMX, Sint32 iMY) {}
void CApp::OnRButtonUp(Sint32 iMX, Sint32 iMY) {}
void CApp::OnMButtonDown(Sint32 iMX, Sint32 iMY) {}
void CApp::OnMButtonUp(Sint32 iMX, Sint32 iMY) {}
void CApp::OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue) {}
void CApp::OnJoyButtonDown(Uint8 yWhich, Uint8 yButton) {}
void CApp::OnJoyButtonUp(Uint8 yWhich, Uint8 yButton) {}
void CApp::OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue) {}
void CApp::OnJoyBall(Uint8 yWhich, Uint8 yBall, Sint16 rRelX, Sint16 yRelY) {}
void CApp::OnMinimize() {}
void CApp::OnRestore() {}
void CApp::OnResize(Sint32 iWidth, Sint32 iHeight) {}
void CApp::OnExpose() {}
void CApp::OnUser(Uint8 rType, Sint32 iCode, void* pData1, void* pData2) {}
