#ifndef _AI_HPP_
#define _AI_HPP_

#include <cstdint>
#include "Player.hpp"
#include "../Grid.hpp"


class AI : public Player
{
    public:
        uint8_t getSearchLimit() const noexcept;

        explicit AI(const Grid::PlayerMark& CEplayerMark = Grid::PlayerMark::GRID_TYPE_NONE, 
            uint8_t ySearchLimit = CHAR_MAX) noexcept;

        void ab_pruning(Grid& grid) const noexcept;

    private:
        uint8_t _ySearchLimit;

        int32_t ab_minValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;
        int32_t ab_maxValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;
        int32_t heuristic(const Grid& Cgrid) const noexcept;
        uint8_t playerMark2Heuristic(const Grid& Cgrid, uint8_t yRow, uint8_t yColumn) const noexcept;
};


inline AI::AI(const Grid::PlayerMark& CEplayerMark, uint8_t ySearchLimit) noexcept : Player{CEplayerMark},
    _ySearchLimit{ySearchLimit} {}
inline uint8_t AI::getSearchLimit() const noexcept { return _ySearchLimit; }


#endif