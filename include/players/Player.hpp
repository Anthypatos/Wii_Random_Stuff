#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Grid.hpp"


class Player
{
    public:
        Grid::PlayerMark getPlayerMark() const noexcept;
        void setPlayerMark(const Grid::PlayerMark& CEplayerMark) noexcept;

        explicit Player(const Grid::PlayerMark& CEplayerMark = Grid::PlayerMark::GRID_TYPE_NONE) noexcept;
        virtual ~Player() noexcept = default;

    protected:
        Grid::PlayerMark __EplayerMark;

};


inline Grid::PlayerMark Player::getPlayerMark() const noexcept { return __EplayerMark; }
inline void Player::setPlayerMark(const Grid::PlayerMark& CEplayerMark) noexcept 
{ __EplayerMark = CEplayerMark; }

inline Player::Player(const Grid::PlayerMark& CEplayerMark) noexcept : __EplayerMark{CEplayerMark} {}


#endif