#ifndef _HUMAN_HPP_
#define _HUMAN_HPP_

#include <cstdint>
#include <SDL_joystick.h>
#include "Player.hpp"
#include "WiiController.hpp"
#include "GameCubeController.hpp"
#include "../Grid.hpp"


class Human : public Player
{
    public:
        WiiController* getWiimote() const noexcept;
        GameCubeController* getGameCubeController() const noexcept;

        explicit Human(const Grid::PlayerMark& CEplayerMark = Grid::PlayerMark::GRID_TYPE_NONE);
        virtual ~Human() noexcept;

    private:
        static const uint8_t _SCmaxWiiJoysticks = 4;
        
        WiiController* _pWiiController;
        GameCubeController* _pGameCubeController;

};


inline WiiController* Human::getWiimote() const noexcept { return _pWiiController; }
inline GameCubeController* Human::getGameCubeController() const noexcept { return _pGameCubeController; }


#endif