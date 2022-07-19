#ifndef _CAPP_HPP_
#define _CAPP_HPP_

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include "../include/CEvent.hpp"
#include "Grid.hpp"


class CApp : public CEvent
{ 
    public:
        static const Uint16 SCrWindowWidth = 640;
        static const Uint16 SCrWindowHeight = 480;
        static Uint8 SyJoysticks;

        static CApp* getInstance();

        CApp(const CApp& CCAppOther) = delete;
        CApp& operator =(const CApp& CCAppOther) = delete;

        void OnExecute();
        void OnInit();
        virtual void OnEvent(SDL_Event* pSdlEvent) noexcept;
        virtual void OnExit() noexcept;
        void OnLoop() const noexcept;
        void OnRender();
        void OnCleanup() const noexcept;
        void Reset() noexcept;

        virtual void OnInputFocus();
        virtual void OnInputBlur();
        virtual void OnKeyDown(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode);
        virtual void OnKeyUp(SDLKey sdlKey, SDLMod sdlMod, Uint16 rUnicode);
        virtual void OnMouseFocus();
        virtual void OnMouseBlur();
        virtual void OnMouseMove(Sint32 iMouseX, Sint32 iMouseY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
            bool bRight, bool bMiddle);
        virtual void OnMouseWheel(bool bUp, bool bDown);
        virtual void OnLButtonDown(Sint32 iMouseX, Sint32 iMouseY);
        virtual void OnLButtonUp(Sint32 iMouseX, Sint32 iMouseY);
        virtual void OnRButtonDown(Sint32 iMouseX, Sint32 iMouseY);
        virtual void OnRButtonUp(Sint32 iMouseX, Sint32 iMouseY);
        virtual void OnMButtonDown(Sint32 iMouseX, Sint32 iMouseY);
        virtual void OnMButtonUp(Sint32 iMouseX, Sint32 iMouseY);
        virtual void OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue);
        virtual void OnJoyButtonDown(Uint8 yWhich, Uint8 yButton);
        virtual void OnJoyButtonUp(Uint8 yWhich, Uint8 yButton);
        virtual void OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue);
        virtual void OnJoyBall(Uint8 yWhich, Uint8 yBall, Sint16 rRelX, Sint16 rRelY);
        virtual void OnMinimize();
        virtual void OnRestore();
        virtual void OnResize(Sint32 iWidth, Sint32 iHeight);
        virtual void OnExpose();
        virtual void OnUser(Uint8 yType, Sint32 iCode, void* pData1, void* pData2);

    private:
        static CApp* _SpCAppInstance; 

        bool _bRunning;
        SDL_Surface* _pSdlSurfaceDisplay;
        SDL_Surface* _pSdlSurfaceGrid;
        SDL_Surface* _pSdlSurfaceX;
        SDL_Surface* _pSdlSurfaceO;

        Grid _grid;
        Grid::PlayerMark _EPlayerMarkCurrent;

        CApp() noexcept;
};

 
#endif