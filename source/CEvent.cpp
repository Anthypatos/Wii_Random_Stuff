#include <SDL_events.h>
#include <SDL_active.h>
#include <SDL_mouse.h>
#include "../include/CEvent.hpp"


CEvent::CEvent() {}


CEvent::~CEvent() {}


void CEvent::OnEvent(SDL_Event* pSdlEvent)
{
    switch(pSdlEvent->type) 
    {
        case SDL_ACTIVEEVENT: 
        {
            switch(pSdlEvent->active.state) 
            {
                case SDL_APPMOUSEFOCUS: 
                {
                    if (pSdlEvent->active.gain) OnMouseFocus();
                    else OnMouseBlur();
 
                    break;
                }
                case SDL_APPINPUTFOCUS: 
                {
                    if (pSdlEvent->active.gain) OnInputFocus();
                    else OnInputBlur();

                    break;
                }
                case SDL_APPACTIVE: 
                {
                    if (pSdlEvent->active.gain) OnRestore();
                    else OnMinimize();
 
                    break;
                }
            }
            break;
        }
 
        case SDL_KEYDOWN: 
        {
            OnKeyDown(pSdlEvent->key.keysym.sym, pSdlEvent->key.keysym.mod, pSdlEvent->key.keysym.unicode);
            break;
        }
 
        case SDL_KEYUP: 
        {
            OnKeyUp(pSdlEvent->key.keysym.sym, pSdlEvent->key.keysym.mod, pSdlEvent->key.keysym.unicode);
            break;
        }
 
        case SDL_MOUSEMOTION: 
        {
            OnMouseMove(pSdlEvent->motion.x, pSdlEvent->motion.y, pSdlEvent->motion.xrel, 
                pSdlEvent->motion.yrel, (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
            break;
        }
 
        case SDL_MOUSEBUTTONDOWN: 
        {
            switch(pSdlEvent->button.button) 
            {
                case SDL_BUTTON_LEFT: 
                {
                    OnLButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: 
                {
                    OnRButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: 
                {
                    OnMButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
            }
            break;
        }
 
        case SDL_MOUSEBUTTONUP: 
        {
            switch(pSdlEvent->button.button) 
            {
                case SDL_BUTTON_LEFT: 
                {
                    OnLButtonUp(pSdlEvent->button.x,pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: 
                {
                    OnRButtonUp(pSdlEvent->button.x,pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: 
                {
                    OnMButtonUp(pSdlEvent->button.x,pSdlEvent->button.y);
                    break;
                }
            }
            break;
        }
 
        case SDL_JOYAXISMOTION: 
        {
            OnJoyAxis(pSdlEvent->jaxis.which, pSdlEvent->jaxis.axis, pSdlEvent->jaxis.value);
            break;
        }
 
        case SDL_JOYBALLMOTION: 
        {
            OnJoyBall(pSdlEvent->jball.which, pSdlEvent->jball.ball, pSdlEvent->jball.xrel,
                pSdlEvent->jball.yrel);
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
 
        case SDL_VIDEORESIZE: 
        {
            OnResize(pSdlEvent->resize.w, pSdlEvent->resize.h);
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
