#include <cstdint>
#include <stdexcept>
#include <SDL_joystick.h>
#include <SDL_error.h>
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/Grid.hpp"


uint8_t Human::_SyJoysticks = 0;


Human::Human(const Grid::PlayerMark& CEplayerMark) : Player{CEplayerMark},
    _pSdlJoystick{nullptr}
{
    if (_SyJoysticks >= SDL_NumJoysticks()) throw std::runtime_error("There are no joysticks available");

    _pSdlJoystick = SDL_JoystickOpen(Human::_SyJoysticks);
    Human::_SyJoysticks++;

    if (_pSdlJoystick == nullptr) throw std::runtime_error(SDL_GetError());
}


Human::~Human() noexcept 
{ 
    SDL_JoystickClose(_pSdlJoystick);
    Human::_SyJoysticks--;
}
