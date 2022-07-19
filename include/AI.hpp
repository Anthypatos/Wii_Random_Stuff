#ifndef AI_HPP_
#define AI_HPP_

#include "Grid.hpp"
#include <cstdint>


class AI
{
    public:
        static uint8_t getSearchLimit() noexcept;
        static void setSearchLimit(uint8_t iSearchLimit) noexcept;

        static int32_t heuristic(const Grid& CGrid) noexcept;
        static void ab_pruning(Grid& grid, const Grid::PlayerMark& CEPlayerMark) noexcept;
        static int32_t ab_minValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, 
            int32_t iDepth, int32_t iAlpha, int32_t iBeta) noexcept;
        static int32_t ab_maxValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, 
            int32_t iDepth, int32_t iAlpha, int32_t iBeta) noexcept;

    private:
        static uint8_t SEARCH_LIMIT;

        AI() = default;
};


inline uint8_t AI::getSearchLimit() noexcept { return AI::SEARCH_LIMIT; }
inline void AI::setSearchLimit(uint8_t iSearchLimit) noexcept { AI::SEARCH_LIMIT = iSearchLimit; }


#endif