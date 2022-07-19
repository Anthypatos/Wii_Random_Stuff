#include <SDL_events.h>
#include "../../include/CApp.hpp"


void CApp::OnEvent(SDL_Event* pSdlEvent) noexcept { CEvent::OnEvent(pSdlEvent); }


void CApp::OnExit() noexcept { _bRunning = false; }
