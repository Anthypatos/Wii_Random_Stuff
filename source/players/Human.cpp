#include <cstdint>
#include <stdexcept>
#include <SDL_joystick.h>
#include <SDL_error.h>
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/Grid.hpp"


Human::Human(const Grid::PlayerMark& CEplayerMark) : Player{CEplayerMark}, _pWiiController{nullptr}, 
    _pGameCubeController{nullptr}
{
    uint8_t yIndex = 0;
    while (yIndex < Human::_SCmaxWiiJoysticks && yIndex < SDL_NumJoysticks() && SDL_JoystickOpened(yIndex))
        yIndex++;

    if (yIndex == Human::_SCmaxWiiJoysticks || yIndex == SDL_NumJoysticks())
        throw std::out_of_range("There are no joysticks available");

    _pWiiController = new WiiController(yIndex);
    _pGameCubeController = new GameCubeController(yIndex);
}


Human::~Human() noexcept 
{ 
    delete _pWiiController;
    delete _pGameCubeController;
}
