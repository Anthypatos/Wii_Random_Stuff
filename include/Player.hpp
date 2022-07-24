#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Grid.hpp"


class Player
{
    public:
        Grid::PlayerMark getPlayerMark() const noexcept;

        explicit Player(const Grid::PlayerMark& CEplayerMark) noexcept;

    protected:
        Grid::PlayerMark __EplayerMark;

};


inline Grid::PlayerMark Player::getPlayerMark() const noexcept { return __EplayerMark; }
inline Player::Player(const Grid::PlayerMark& CEplayerMark) noexcept : __EplayerMark{CEplayerMark} {}


#endif