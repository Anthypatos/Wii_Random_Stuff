#include <climits>
#include <algorithm>
#include <cstdint>
#include "../include/AI.hpp"
#include "../include/Grid.hpp"


uint8_t AI::SEARCH_LIMIT = CHAR_MAX;


int32_t AI::heuristic(const Grid& CGrid) noexcept
{
    return 0; // TO DO
}


void AI::ab_pruning(Grid& grid, const Grid::PlayerMark& CEPlayerMark) noexcept
{
    int32_t iAlpha = INT_MIN, iBeta = INT_MAX, iDepth = 1, iBestPlayRow = 0, iBestPlayColumn = 0, 
        iValue = 0;

    for (uint8_t i = 0; i < grid.getWidth(); i++)
    {
        for (uint8_t j = 0; j < grid.getHeight(); j++)
        {
            if (grid.isValidPlay(i, j))
            {
                Grid gridAttempt = grid;
                gridAttempt.makePlay(CEPlayerMark, i, j);
                iValue = AI::ab_minValue(gridAttempt, Grid::nextPlayer(CEPlayerMark), iDepth + 1, 
                    iAlpha, iBeta);

                if (iValue > iAlpha)
                {
                    iAlpha = iValue;
                    iBestPlayRow = i, iBestPlayColumn = j;
                }
            }
        }
    }

    if (grid.isValidPlay(iBestPlayRow, iBestPlayColumn)) 
        grid.makePlay(CEPlayerMark, iBestPlayRow, iBestPlayColumn);
}


int32_t AI::ab_minValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) noexcept
{
    int8_t yWinner = CGrid.endResult();

    if (yWinner == Grid::PlayerMark::GRID_TYPE_O || yWinner == Grid::PlayerMark::GRID_TYPE_X)
        return yWinner;
    else if (iDepth == AI::SEARCH_LIMIT) return AI::heuristic(CGrid);
    else
    {
        for (uint8_t i = 0; i < CGrid.getWidth() && iAlpha < iBeta; i++)
        {
            for (uint8_t j = 0; j < CGrid.getHeight() && iAlpha < iBeta; j++)
            {
                if (CGrid.isValidPlay(i, j))
                {
                    Grid gridAttempt = CGrid;
                    gridAttempt.makePlay(CEPlayerMark, i, j);
                    iBeta = std::min(iBeta, ab_maxValue(gridAttempt, Grid::nextPlayer(CEPlayerMark), 
                        iDepth + 1, iAlpha, iBeta));
                }
            }
        }
        return iBeta;
    }
}


int32_t AI::ab_maxValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) noexcept
{
    int8_t yWinner = CGrid.endResult();

    if (yWinner == Grid::PlayerMark::GRID_TYPE_O || yWinner == Grid::PlayerMark::GRID_TYPE_X)
        return yWinner;
    else if (iDepth == AI::SEARCH_LIMIT) return AI::heuristic(CGrid);
    else
    {
        for (uint8_t i = 0; i < CGrid.getWidth() && iAlpha < iBeta; i++)
        {
            for (uint8_t j = 0; j < CGrid.getHeight() && iAlpha < iBeta; j++)
            {
                if (CGrid.isValidPlay(i, j))
                {
                    Grid gridAttempt = CGrid;
                    gridAttempt.makePlay(CEPlayerMark, i, j);
                    iAlpha = std::max(iAlpha, ab_minValue(gridAttempt, Grid::nextPlayer(CEPlayerMark), 
                        iDepth + 1, iAlpha, iBeta));
                }
            }
        }
        return iAlpha;
    }
}
