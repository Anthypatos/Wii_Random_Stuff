#ifndef _CAPP_HPP_
#define _CAPP_HPP_

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include "../include/CEvent.hpp"


#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

/* Define an array of pointer of joysticks, for games with one player, use just a 
    SDL_Joystick* SDL_NumJoysticks and a malloc is also a good alternative for variable 
    number of joysticks at runtime (not usual though)*/
#define JOYNUMS 4


class CApp : public CEvent
{ 
    public:
        static CApp* getInstance();

        CApp(const CApp& CCAppOther) = delete;
        CApp& operator =(const CApp& CCAppOther) = delete;

        void OnExecute();
        void OnInit();
        virtual void OnEvent(SDL_Event* pSdlEvent) noexcept;
        virtual void OnExit() noexcept;
        void OnLoop() const noexcept;
        void OnRender() const noexcept;
        void OnCleanup() const noexcept;

        virtual void OnInputFocus();
        virtual void OnInputBlur();
        virtual void OnKeyDown(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode);
        virtual void OnKeyUp(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode);
        virtual void OnMouseFocus();
        virtual void OnMouseBlur();
        virtual void OnMouseMove(Sint32 iMX, Sint32 iMY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
            bool bRight, bool bMiddle);
        virtual void OnMouseWheel(bool bUp, bool bDown);
        virtual void OnLButtonDown(Sint32 iMX, Sint32 iMY);
        virtual void OnLButtonUp(Sint32 iMX, Sint32 iMY);
        virtual void OnRButtonDown(Sint32 iMX, Sint32 iMY);
        virtual void OnRButtonUp(Sint32 iMX, Sint32 iMY);
        virtual void OnMButtonDown(Sint32 iMX, Sint32 iMY);
        virtual void OnMButtonUp(Sint32 iMX, Sint32 iMY);
        virtual void OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue);
        virtual void OnJoyButtonDown(Uint8 yWhich, Uint8 yButton);
        virtual void OnJoyButtonUp(Uint8 yWhich, Uint8 yButton);
        virtual void OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue);
        virtual void OnJoyBall(Uint8 yWhich, Uint8 yBall, Sint16 rRelX, Sint16 yRelY);
        virtual void OnMinimize();
        virtual void OnRestore();
        virtual void OnResize(Sint32 iWidth, Sint32 iHeight);
        virtual void OnExpose();
        virtual void OnUser(Uint8 rType, Sint32 iCode, void* pData1, void* pData2);

    protected:
        bool __bRunning;
        SDL_Surface* __pSdlSurfaceDisplay;
        SDL_Surface* __pSdlSurfaceTest;

    private:
        static CApp* _SpCAppInstance; 

        CApp() noexcept;
};

 
#endif