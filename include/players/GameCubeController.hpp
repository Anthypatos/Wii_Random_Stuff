#include <cstdint>
#include <SDL_joystick.h>
#include "Joystick.hpp"


class GameCubeController : public Joystick
{
    public:
        explicit GameCubeController(uint8_t yIndex);
        GameCubeController(const GameCubeController& CgameCubeController) = delete;

        GameCubeController& operator =(const GameCubeController& CgameCubeController) = delete;

        uint8_t getButtonX() const noexcept;
        uint8_t getButtonY() const noexcept;
        uint8_t getButtonZ() const noexcept;
        uint8_t getButtonR() const noexcept;
        uint8_t getButtonL() const noexcept;
        uint8_t getButtonStart() const noexcept;
        
};


inline GameCubeController::GameCubeController(uint8_t yIndex) : Joystick{yIndex + 4}
{}
inline uint8_t GameCubeController::getButtonX() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 2); }
inline uint8_t GameCubeController::getButtonY() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 3); }
inline uint8_t GameCubeController::getButtonZ() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 4); }
inline uint8_t GameCubeController::getButtonR() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 5); }
inline uint8_t GameCubeController::getButtonL() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 6); }
inline uint8_t GameCubeController::getButtonStart() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 7); }
