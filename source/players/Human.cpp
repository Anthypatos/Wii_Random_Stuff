#include <string>
#include "../../include/players/Human.hpp"
#include "../../include/players/Player.hpp"


Human::Human(const Grid::PlayerMark& CEplayerMark) : Player{CEplayerMark},
    _pSdlJoystick{SDL_JoystickOpen(SDL_NumJoysticks() + 1)} 
{ 
    if (_pSdlJoystick == nullptr) 
        throw std::runtime_error(std::string("Unable to open joystick: ") + SDL_GetError()); 
}


Human::~Human() noexcept 
{
    SDL_JoystickClose(_pSdlJoystick);
    Player::~Player();
}
