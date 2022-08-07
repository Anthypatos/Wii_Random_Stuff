#ifndef _WIICONTROLLER_HPP_
#define _WIICONTROLLER_HPP_

#include <cstdint>
#include <SDL_joystick.h>
#include <SDL_mouse.h>
#include "Joystick.hpp"


class WiiController : public Joystick
{
    public:
        explicit WiiController(uint8_t yIndex);
        WiiController(const WiiController& CwiiController) = delete;

        WiiController& operator =(const WiiController& CwiiController) = delete;

        int16_t getRightTriggerClassic() const noexcept;
        int16_t getLeftTriggerClassic() const noexcept;
        int16_t getPitch() const noexcept;
        int16_t getRoll() const noexcept;
        int16_t getYaw() const noexcept;
        bool getButton1() const noexcept;
        bool getButton2() const noexcept;
        bool getButtonMinus() const noexcept;
        bool getButtonPlus() const noexcept;
        bool getButtonHOME() const noexcept;
        bool getButtonZ() const noexcept;
        bool getButtonC() const noexcept;
        bool getButtonAClassic() const noexcept;
        bool getButtonBClassic() const noexcept;
        bool getButtonXClassic() const noexcept;
        bool getButtonYClassic() const noexcept;
        bool getButtonLClassic() const noexcept;
        bool getButtonRClassic() const noexcept;
        bool getButtonZLClassic() const noexcept;
        bool getButtonZRClassic() const noexcept;
        bool getButtonMinusClassic() const noexcept;
        bool getButtonPlusClassic() const noexcept;
        bool getButtonHOMEClassic() const noexcept;
        void getIRPointer(int32_t* pMouseX, int32_t* pMouseY) const noexcept;

};


inline WiiController::WiiController(uint8_t yIndex) : Joystick{yIndex} 
{}
inline int16_t WiiController::getRightTriggerClassic() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 4); }
inline int16_t WiiController::getLeftTriggerClassic() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 5); }
inline int16_t WiiController::getPitch() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 6); }
inline int16_t WiiController::getRoll() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 7); }
inline int16_t WiiController::getYaw() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 8); }
inline bool WiiController::getButton1() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 2); }
inline bool WiiController::getButton2() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 3); }
inline bool WiiController::getButtonMinus() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 4); }
inline bool WiiController::getButtonPlus() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 5); }
inline bool WiiController::getButtonHOME() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 6); }
inline bool WiiController::getButtonZ() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 7); }
inline bool WiiController::getButtonC() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 8); }
inline bool WiiController::getButtonAClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 9); }
inline bool WiiController::getButtonBClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 10); }
inline bool WiiController::getButtonXClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 11); }
inline bool WiiController::getButtonYClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 12); }
inline bool WiiController::getButtonLClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 13); }
inline bool WiiController::getButtonRClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 14); }
inline bool WiiController::getButtonZLClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 15); }
inline bool WiiController::getButtonZRClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 16); }
inline bool WiiController::getButtonMinusClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 17); }
inline bool WiiController::getButtonPlusClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 18); }
inline bool WiiController::getButtonHOMEClassic() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 19); }
inline void WiiController::getIRPointer(int32_t* pMouseX, int32_t* pMouseY) const noexcept
{ SDL_GetMouseState(pMouseX, pMouseY); }


#endif