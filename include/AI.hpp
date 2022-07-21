#ifndef AI_HPP_
#define AI_HPP_

#include "Grid.hpp"
#include <cstdint>


class AI
{
    public:
        Grid::PlayerMark getPlayerMark() const noexcept;
        uint8_t getSearchLimit() const noexcept;

        AI(const Grid::PlayerMark& CEPlayerMark, uint8_t ySearchLimit = CHAR_MAX) noexcept;

        int32_t heuristic(const Grid& CGrid) noexcept;
        void ab_pruning(Grid& grid, const Grid::PlayerMark& CEPlayerMark) noexcept;
        int32_t ab_minValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) noexcept;
        int32_t ab_maxValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) noexcept;

    private:
        Grid::PlayerMark _EPlayerMark;
        uint8_t _ySearchLimit;

        static uint8_t playerMark2Heuristic(const Grid::PlayerMark& CEPlayerMarkOwn, const Grid& CGrid,
            uint8_t yRow, uint8_t yColumn) noexcept;
};


inline Grid::PlayerMark AI::getPlayerMark() const noexcept { return _EPlayerMark; }
inline uint8_t AI::getSearchLimit() const noexcept { return _ySearchLimit; }


#endif