#ifndef _JOYSTICK_HPP_
#define _JOYSTICK_HPP_

#include <cstdint>
#include <set>
#include <SDL_joystick.h>
#include "Human.hpp"


class Joystick
{
    public:
        explicit Joystick(uint8_t yIndex);
        ~Joystick() noexcept;

        void associateUser(Human& human) noexcept;

    private:
        SDL_Joystick* _pSdlJoystick;
        std::set<Human*> _setpHumanUsers;

};


#endif