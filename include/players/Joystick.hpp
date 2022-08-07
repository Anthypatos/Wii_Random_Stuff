#ifndef _JOYSTICK_HPP_
#define _JOYSTICK_HPP_

#include <cstdint>
#include <SDL_joystick.h>


class Joystick
{
    public:
        explicit Joystick(int32_t yIndex);
        virtual ~Joystick() noexcept;

        int32_t getIndex() const noexcept;
        int16_t getLeftStickHorizontal() const noexcept;
        int16_t getLeftStickVertical() const noexcept;
        int16_t getRightStickHorizontal() const noexcept;
        int16_t getRightStickVertical() const noexcept;
        uint8_t getDPadState() const noexcept;
        uint8_t getButtonA() const noexcept;
        uint8_t getButtonB() const noexcept;

    protected:
        SDL_Joystick* __pSdlJoystick;

};


inline int32_t Joystick::getIndex() const noexcept
{ return SDL_JoystickIndex(__pSdlJoystick); }
inline int16_t Joystick::getLeftStickHorizontal() const noexcept 
{ return SDL_JoystickGetAxis(__pSdlJoystick, 0); }
inline int16_t Joystick::getLeftStickVertical() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 1); }
inline int16_t Joystick::getRightStickHorizontal() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 2); }
inline int16_t Joystick::getRightStickVertical() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 3); }
inline uint8_t Joystick::getDPadState() const noexcept 
{ return SDL_JoystickGetHat(__pSdlJoystick, 0); }
inline uint8_t Joystick::getButtonA() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 0); }
inline uint8_t Joystick::getButtonB() const noexcept 
{ return SDL_JoystickGetButton(__pSdlJoystick, 1); }


#endif