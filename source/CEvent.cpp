#include <SDL_events.h>
#include <SDL_active.h>
#include <SDL_mouse.h>
#include "../include/CEvent.hpp"


void CEvent::OnEvent(SDL_Event* pSdlEvent)
{
    switch(pSdlEvent->type)
    {
        case SDL_MOUSEBUTTONDOWN:
            OnLButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
            break;
        case SDL_MOUSEMOTION:
        {
            OnMouseMove(pSdlEvent->motion.x, pSdlEvent->motion.y, pSdlEvent->motion.xrel,
                pSdlEvent->motion.yrel, (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
            break;
        }

        case SDL_JOYAXISMOTION:
        {
            OnJoyAxis(pSdlEvent->jaxis.which, pSdlEvent->jaxis.axis, pSdlEvent->jaxis.value);
            break;
        }

        case SDL_JOYHATMOTION:
        {
            OnJoyHat(pSdlEvent->jhat.which, pSdlEvent->jhat.hat, pSdlEvent->jhat.value);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        {
            OnJoyButtonDown(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
            break;
        }

        case SDL_JOYBUTTONUP:
        {
            OnJoyButtonUp(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
            break;
        }

        case SDL_QUIT:
        {
            OnExit();
            break;
        }

        case SDL_SYSWMEVENT:
        {
            //Ignore
            break;
        }

        case SDL_VIDEOEXPOSE:
        {
            OnExpose();
            break;
        }

        default:
        {
            OnUser(pSdlEvent->user.type, pSdlEvent->user.code, pSdlEvent->user.data1,
                pSdlEvent->user.data2);
            break;
        }
    }
}
