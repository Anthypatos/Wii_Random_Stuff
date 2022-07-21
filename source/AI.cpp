#include <climits>
#include <algorithm>
#include <cstdint>
#include "../include/AI.hpp"
#include "../include/Grid.hpp"


AI::AI(const Grid::PlayerMark& CEPlayerMark, uint8_t ySearchLimit) noexcept : _EPlayerMark{CEPlayerMark},
    _ySearchLimit{ySearchLimit}
{}


void AI::ab_pruning(Grid& grid) const noexcept
{
    int32_t iAlpha = INT_MIN, iBeta = INT_MAX, iDepth = 1, iBestPlayRow = 0, iBestPlayColumn = 0, 
        iValue = 0;

    for (uint8_t i = 0; i < grid.getHeight(); i++)
    {
        for (uint8_t j = 0; j < grid.getWidth(); j++)
        {
            if (grid.isValidPlay(i, j))
            {
                Grid gridAttempt = grid;
                gridAttempt.makePlay(_EPlayerMark, i, j);
                iValue = AI::ab_minValue(gridAttempt, Grid::nextPlayer(_EPlayerMark), iDepth + 1, 
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
        grid.makePlay(_EPlayerMark, iBestPlayRow, iBestPlayColumn);
}


int32_t AI::ab_minValue(const Grid& CGrid, const Grid::PlayerMark& CEPlayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int8_t yWinner = CGrid.endResult();

    if (yWinner == Grid::PlayerMark::GRID_TYPE_O || yWinner == Grid::PlayerMark::GRID_TYPE_X)
        return yWinner;
    else if (iDepth == _ySearchLimit) return AI::heuristic(CGrid);
    else
    {
        for (uint8_t i = 0; i < CGrid.getHeight() && iAlpha < iBeta; i++)
        {
            for (uint8_t j = 0; j < CGrid.getWidth() && iAlpha < iBeta; j++)
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
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int8_t yWinner = CGrid.endResult();

    if (yWinner == Grid::PlayerMark::GRID_TYPE_O || yWinner == Grid::PlayerMark::GRID_TYPE_X)
        return yWinner;
    else if (iDepth == _ySearchLimit) return AI::heuristic(CGrid);
    else
    {
        for (uint8_t i = 0; i < CGrid.getHeight() && iAlpha < iBeta; i++)
        {
            for (uint8_t j = 0; j < CGrid.getWidth() && iAlpha < iBeta; j++)
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


int32_t AI::heuristic(const Grid& CGrid) const noexcept
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
                // Vertical up check
                if (i >= yMatchNumber && (i == yHeight - 1 || CGrid[i][j] != CGrid[i + 1][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i - yOffset][j] == CGrid[i][j] ||
                        CGrid[i - yOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i - yOffset][j] == CGrid[i][j]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Vertical down check
                if (i <= yHeight - yMatchNumber && (i == 0 || CGrid[i][j] != CGrid[i - 1][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i + yOffset][j] == CGrid[i][j] ||
                        CGrid[i + yOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i + yOffset][j] == CGrid[i][j]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Horizontal left check
                if (j >= yMatchNumber && (j == yWidth - 1 || CGrid[i][j] != CGrid[i][j + 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i][j - yOffset] == CGrid[i][j] ||
                        CGrid[i][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i][j - yOffset] == CGrid[i][j]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Horizontal right check
                if (j <= yWidth - yMatchNumber && (j == 0 || CGrid[i][j] != CGrid[i][j - 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i][j + yOffset] == CGrid[i][j] ||
                        CGrid[i][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i][j] == CGrid[i][j + yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Diagonal up right check
                if (i >= yMatchNumber && j <= yWidth - yMatchNumber &&
                    (i == yHeight - 1 || j == 0 || CGrid[i][j] != CGrid[i - 1][j - 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i + yOffset][j + yOffset] == CGrid[i][j] ||
                        CGrid[i + yOffset][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i][j] == CGrid[i + yOffset][j + yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Diagonal down left check
                if (i <= yHeight - yMatchNumber && j >= yMatchNumber &&
                    (i == 0 || j == yWidth - 1 || CGrid[i][j] != CGrid[i - 1][j + 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i + yOffset][j - yOffset] == CGrid[i][j] ||
                        CGrid[i + yOffset][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i][j] == CGrid[i + yOffset][j - yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Diagonal up left check
                if (i >= yMatchNumber && j >= yMatchNumber &&
                    (i == yHeight - 1 || j == yWidth - 1 || CGrid[i][j] != CGrid[i + 1][j + 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i - yOffset][j - yOffset] == CGrid[i][j] ||
                        CGrid[i - yOffset][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i][j] == CGrid[i - yOffset][j - yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
                // Diagonal down right check
                if (i <= yHeight - yMatchNumber && j <= yWidth - yMatchNumber &&
                    (i == 0 || j == 0 || CGrid[i][j] != CGrid[i - 1][j - 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < yMatchNumber && (CGrid[i + yOffset][j + yOffset] == CGrid[i][j] ||
                        CGrid[i + yOffset][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (CGrid[i][j] == CGrid[i + yOffset][j + yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == yMatchNumber) return CGrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(CGrid, i, j) * yCounter);
                }
            }
        }
    }
    return iHeuristic;
}


uint8_t AI::playerMark2Heuristic(const Grid& CGrid, uint8_t yRow, uint8_t yColumn) const noexcept
{
    if (CGrid[yRow][yColumn] == _EPlayerMark) return 1;
    else return - 1;
}
