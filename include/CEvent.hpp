#ifndef _CEVENT_HPP_
#define _CEVENT_HPP_
 
#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_events.h>
#include <SDL_keysym.h>
 
class CEvent 
{
    public:
        CEvent();
        virtual ~CEvent();
 
        virtual void OnEvent(SDL_Event* pSdlEvent);
        virtual void OnInputFocus() = 0;
        virtual void OnInputBlur() = 0;
        virtual void OnKeyDown(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode) = 0;
        virtual void OnKeyUp(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode) = 0;
        virtual void OnMouseFocus() = 0;
        virtual void OnMouseBlur() = 0;
        virtual void OnMouseMove(Sint32 iMX, Sint32 iMY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
            bool bRight, bool bMiddle) = 0;
        virtual void OnMouseWheel(bool bUp, bool bDown) = 0;
        virtual void OnLButtonDown(Sint32 iMX, Sint32 iMY) = 0;
        virtual void OnLButtonUp(Sint32 iMX, Sint32 iMY) = 0;
        virtual void OnRButtonDown(Sint32 iMX, Sint32 iMY) = 0;
        virtual void OnRButtonUp(Sint32 iMX, Sint32 iMY) = 0;
        virtual void OnMButtonDown(Sint32 iMX, Sint32 iMY) = 0;
        virtual void OnMButtonUp(Sint32 iMX, Sint32 iMY) = 0;
        virtual void OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue) = 0;
        virtual void OnJoyButtonDown(Uint8 yWhich, Uint8 yButton) = 0;
        virtual void OnJoyButtonUp(Uint8 yWhich, Uint8 yButton) = 0;
        virtual void OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue) = 0;
        virtual void OnJoyBall(Uint8 yWhich, Uint8 yBall, Sint16 rRelX, Sint16 yRelY) = 0;
        virtual void OnMinimize() = 0;
        virtual void OnRestore() = 0;
        virtual void OnResize(Sint32 iWidth, Sint32 iHeight) = 0;
        virtual void OnExpose() = 0;
        virtual void OnExit() = 0;
        virtual void OnUser(Uint8 rType, Sint32 iCode, void* pData1, void* pData2) = 0;
};
 
#endif