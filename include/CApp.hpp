#ifndef _CAPP_HPP_
#define _CAPP_HPP_

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include "CEvent.hpp"
#include "Grid.hpp"
#include "players/Player.hpp"


class CApp : public CEvent
{ 
    public:
        enum State_t {STATE_START, STATE_INGAME, STATE_WIN};

        static const Uint16 SCrWindowWidth = 640;
        static const Uint16 SCrWindowHeight = 480;

        static CApp* getInstance();

        CApp(const CApp& CCAppOther) = delete;
        CApp& operator =(const CApp& CCAppOther) = delete;

        void OnExecute();
        void OnInit();
        void OnLoop() const noexcept;
        void OnRender();
        void OnCleanup() const noexcept;
        void Reset() noexcept;

        virtual void OnEvent(SDL_Event* pSdlEvent) noexcept;
        virtual void OnMouseMove(Sint32 iMouseX, Sint32 iMouseY, Sint32 iRelX, Sint32 iRelY, 
            bool bLeft, bool bRight, bool bMiddle) noexcept;
        virtual void OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue) noexcept;
        virtual void OnJoyButtonDown(Uint8 yWhich, Uint8 yButton) noexcept;
        virtual void OnJoyButtonUp(Uint8 yWhich, Uint8 yButton) noexcept;
        virtual void OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue) noexcept;
        virtual void OnExit() noexcept;
        virtual void OnExpose() noexcept;
        virtual void OnUser(Uint8 yType, Sint32 iCode, void* pData1, void* pData2) noexcept;

    private:
        static CApp* _SpCAppInstance; 

        bool _bRunning;
        State_t _ECurrentState;
        
        SDL_Surface* _pSdlSurfaceDisplay;
        SDL_Surface* _pSdlSurfaceStart;
        SDL_Surface* _pSdlSurfaceGrid;
        SDL_Surface* _pSdlSurfaceRed;
        SDL_Surface* _pSdlSurfaceYellow;
        SDL_Surface* _pSdlSurfaceWinRed;
        SDL_Surface* _pSdlSurfaceWinYellow;

        Grid _grid;
        Grid::PlayerMark _EplayerMarkCurrent;
        Player* _apPlayer;

        CApp() noexcept;
};

 
#endif