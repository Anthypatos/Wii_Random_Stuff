#include <cstdint>
#include <set>
#include <exception>
#include <stdexcept>
#include <SDL_joystick.h>
#include "../../include/players/Joystick.hpp"
#include "../../include/players/Human.hpp"


Joystick::Joystick(uint8_t yIndex) : _pSdlJoystick{nullptr}, _setpHumanUsers{std::set<Human*>()}
{
    if (yIndex >= SDL_NumJoysticks()) 
        throw std::out_of_range("There is not a joystick available with the selected index");

    if (SDL_JoystickOpened(yIndex)) throw std::logic_error("The joystick is already open");

    if ((_pSdlJoystick = SDL_JoystickOpen(yIndex)) == nullptr) throw std::runtime_error(SDL_GetError());
}


Joystick::~Joystick() noexcept { SDL_JoystickClose(_pSdlJoystick); }


void Joystick::associateUser(Human& human) noexcept { _setpHumanUsers.insert(&human); }
