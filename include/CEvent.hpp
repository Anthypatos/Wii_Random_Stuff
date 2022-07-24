#ifndef _CEVENT_HPP_
#define _CEVENT_HPP_
 
#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_events.h>
#include <SDL_keysym.h>
 
class CEvent 
{
    public:
        virtual ~CEvent() = default;
 
        virtual void OnEvent(SDL_Event* pSdlEvent);
        virtual void OnMouseMove(Sint32 iMouseX, Sint32 iMouseY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
            bool bRight, bool bMiddle) = 0;
        virtual void OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue) = 0;
        virtual void OnJoyButtonDown(Uint8 yWhich, Uint8 yButton) = 0;
        virtual void OnJoyButtonUp(Uint8 yWhich, Uint8 yButton) = 0;
        virtual void OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue) = 0;
        virtual void OnExpose() = 0;
        virtual void OnExit() = 0;
        virtual void OnUser(Uint8 yType, Sint32 iCode, void* pData1, void* pData2) = 0;
};
 
#endif