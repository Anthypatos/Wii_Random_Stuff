#include <climits>
#include <algorithm>
#include <cstdint>
#include "../include/AI.hpp"
#include "../include/Grid.hpp"


uint8_t AI::SEARCH_LIMIT = CHAR_MAX;


int32_t AI::heuristic(const Grid& CGrid) noexcept
{
    uint8_t yHeight = CGrid.getHeight();
    uint8_t yWidth = CGrid.getWidth();
    uint8_t yMatchNumber = CGrid.getMatchNumber();
    uint32_t iHeuristic = 0;

    for (uint8_t i = 0; i < yHeight; i++)
    {
        for (uint8_t j = 0; j < yWidth; j++)
        {
            if (CGrid[i][j] != Grid::PlayerMark::GRID_TYPE_NONE)
            {
                // Vertical check
                if (i <= yHeight - yMatchNumber &&
                    (i == 0 || CGrid[i - 1][j] != CGrid[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && CGrid[i][j] == CGrid[i + yOffset][j])
                        yCounter++, yOffset++;

                    if (yCounter == 4) return CGrid[i][j];
                    else if ((i != 0 && CGrid[i - 1][j] == Grid::PlayerMark::GRID_TYPE_NONE) ||
                        CGrid[i + yOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE)
                        iHeuristic += yCounter * 
                            AI::playerMark2Heuristic(Grid::PlayerMark::GRID_TYPE_X, CGrid, i, j);
                }
                // Horizontal check
                if (j <= yWidth - yMatchNumber && 
                    (j == 0 || CGrid[i][j - 1] != CGrid[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && CGrid[i][j] == CGrid[i][j + yOffset])
                        yCounter++, yOffset++;

                    if (yCounter == 4) return CGrid[i][j];
                    else if ((j != 0 && CGrid[i][j - 1] == Grid::PlayerMark::GRID_TYPE_NONE) ||
                        CGrid[i][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE)
                        iHeuristic += yCounter * 
                            AI::playerMark2Heuristic(Grid::PlayerMark::GRID_TYPE_X, CGrid, i, j);
                }
                // Diagonal left check
                if (i <= yHeight - yMatchNumber && j >= yMatchNumber &&
                    (i == 0 || j == yWidth - 1 || 
                    CGrid[i - 1][j + 1] != CGrid[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && 
                        CGrid[i][j] == CGrid[i + yOffset][j - yOffset])
                        yCounter++, yOffset++;

                    if (yCounter == 4) return CGrid[i][j];
                    else if ((i != 0 && j != yWidth - 1 && 
                        CGrid[i - 1][j + 1] == Grid::PlayerMark::GRID_TYPE_NONE) ||
                        CGrid[i + yOffset][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE)
                        iHeuristic += yCounter * 
                            AI::playerMark2Heuristic(Grid::PlayerMark::GRID_TYPE_X, CGrid, i, j);
                }
                // Diagonal right check
                if (i <= yHeight - yMatchNumber && 
                    j <= yWidth - yMatchNumber &&
                    (i == 0 || j == 0 || CGrid[i - 1][j - 1] != CGrid[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && 
                        CGrid[i][j] == CGrid[i + yOffset][j + yOffset])
                        yCounter++, yOffset++;

                    if (yCounter == 4) return CGrid[i][j];
                    else if ((i != 0 && j != 0 && 
                        CGrid[i - 1][j - 1] == Grid::PlayerMark::GRID_TYPE_NONE) ||
                        CGrid[i + yOffset][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE)
                        iHeuristic += yCounter * 
                            AI::playerMark2Heuristic(Grid::PlayerMark::GRID_TYPE_X, CGrid, i, j);
                }
            }
        }
    }
    return iHeuristic;
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


uint8_t AI::playerMark2Heuristic(const Grid::PlayerMark& CEPlayerMarkOwn, const Grid& CGrid, 
    uint8_t yRow, uint8_t yColumn) noexcept
{
    if (CGrid[yRow][yColumn] == CEPlayerMarkOwn) return 1;
    else return - 1;
}
