#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include "../../include/CApp.hpp"


void CApp::OnEvent(SDL_Event* pSdlEvent) noexcept { CEvent::OnEvent(pSdlEvent); }


void CApp::OnMouseMove(Sint32 iMouseX, Sint32 iMouseY, Sint32 iRelX, Sint32 iRelY, bool bLeft, 
    bool bRight, bool bMiddle) noexcept {}


void CApp::OnJoyAxis(Uint8 yWhich, Uint8 axis, Sint16 rValue) noexcept {}


void CApp::OnJoyButtonDown(Uint8 yWhich, Uint8 yButton) noexcept
{
    switch (_ECurrentState)
    {
        case State_t::STATE_START:
        {
            switch (yButton)
            {
                case 0:
                {
                    Sint32 iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    // if (coordenadas en el boton) 
                    //     _EcurrentState = State_t::STATE_INGAME;
                    //     delete[] _apPlayer;
                    //     _apPlayer = new Player[2];
                    //     for (Uint8 i = 0; i < SDL_NumJoysticks(); i++) _apPlayer[i].setPlayerMark(...);
                    // endif

                    break;
                }
                case 6: _bRunning = false; break;
            }

            break;
        }
        case State_t::STATE_INGAME:
        {
            if (_apPlayer[yWhich].getPlayerMark() == _EplayerMarkCurrent)
            {
                switch (yButton)
                {
                    case 0:
                    {
                        Sint32 iMouseX = 0, iMouseY = 0;
                        SDL_GetMouseState(&iMouseX, &iMouseY);
                        Sint32 iColumn = iMouseX / (_pSdlSurfaceDisplay->w / Grid::SCyWidth);
                
                        if (_grid.isValidPlay(iColumn))
                        {
                            _grid.makePlay(_EplayerMarkCurrent, iColumn);
                            if (_grid.checkWinner() == Grid::PlayerMark::GRID_TYPE_NONE) 
                                _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                            else _ECurrentState = State_t::STATE_WIN;
                        }

                        break;
                    }
                    case 6: _bRunning = false; break;
                }

                break;
            }
        }
        case State_t::STATE_WIN:
        {
            switch (yButton)
            {
                case 0:
                {
                    Sint32 iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    // if (coordenadas en el boton) Reset();

                    break;
                }
                case 6: _bRunning = false; break;
            }

            break;
        }
    }
}


void CApp::OnJoyButtonUp(Uint8 yWhich, Uint8 yButton) noexcept {}


void CApp::OnJoyHat(Uint8 yWhich, Uint8 yHat, Uint8 yValue) noexcept {}


void CApp::OnExpose() noexcept {}


void CApp::OnUser(Uint8 yType, Sint32 iCode, void* pData1, void* pData2) noexcept {}


void CApp::OnExit() noexcept { _bRunning = false; }
