#ifndef _CAPP_HPP_
#define _CAPP_HPP_

#include <cstdint>
#include <vector>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include "CEvent.hpp"
#include "Grid.hpp"
#include "players/Player.hpp"


class CApp : public CEvent
{
    public:
        enum State_t {STATE_START, STATE_INGAME, STATE_WIN};

        static const uint16_t SCrWindowWidth = 640;
        static const uint16_t SCrWindowHeight = 480;

        static CApp* getInstance();

        CApp(const CApp& CCAppOther) = delete;
        CApp& operator =(const CApp& CCAppOther) = delete;

        void OnExecute();
        void OnInit();
        void OnLoop() const noexcept;
        void OnRender();
        void OnCleanup() noexcept;
        void Reset() noexcept;

        virtual void OnLButtonDown(int32_t iMouseX, int32_t iMouseY);

        virtual void OnEvent(SDL_Event* pSdlEvent) noexcept;
        virtual void OnMouseMove(int32_t iMouseX, int32_t iMouseY, int32_t iRelX, int32_t iRelY,
            bool bLeft, bool bRight, bool bMiddle) noexcept;
        virtual void OnJoyAxis(uint8_t yWhich, uint8_t axis, Sint16 rValue) noexcept;
        virtual void OnJoyButtonDown(uint8_t yWhich, uint8_t yButton) noexcept;
        virtual void OnJoyButtonUp(uint8_t yWhich, uint8_t yButton) noexcept;
        virtual void OnJoyHat(uint8_t yWhich, uint8_t yHat, uint8_t yValue) noexcept;
        virtual void OnExit() noexcept;
        virtual void OnExpose() noexcept;
        virtual void OnUser(uint8_t yType, int32_t iCode, void* pData1, void* pData2) noexcept;

    private:
        static CApp* _SpCAppInstance;

        bool _bRunning;
        State_t _EcurrentState;

        SDL_Surface* _pSdlSurfaceDisplay;
        SDL_Surface* _pSdlSurfaceStart;
        SDL_Surface* _pSdlSurfaceGrid;
        SDL_Surface* _pSdlSurfaceRed;
        SDL_Surface* _pSdlSurfaceYellow;
        SDL_Surface* _pSdlSurfaceWinRed;
        SDL_Surface* _pSdlSurfaceWinYellow;

        Grid _grid;
        Grid::PlayerMark _EplayerMarkCurrent;
        std::vector<Player*> _apPlayer;

        CApp() noexcept;
};


#endif
