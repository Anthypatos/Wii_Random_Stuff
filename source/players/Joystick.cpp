#include <cstdint>
#include <exception>
#include <stdexcept>
#include <SDL_joystick.h>
#include "../../include/players/Joystick.hpp"
#include "../../include/players/Human.hpp"


Joystick::Joystick(int32_t yIndex) : __pSdlJoystick{nullptr}
{
    if (yIndex >= SDL_NumJoysticks()) 
        throw std::out_of_range("There is not a joystick available with the selected index");

    if (SDL_JoystickOpened(yIndex)) throw std::logic_error("The joystick is already open");

    if ((__pSdlJoystick = SDL_JoystickOpen(yIndex)) == nullptr) throw std::runtime_error(SDL_GetError());
}


Joystick::~Joystick() noexcept { SDL_JoystickClose(__pSdlJoystick); }
