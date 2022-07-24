#include <climits>
#include <algorithm>
#include <cstdint>
#include "../include/AI.hpp"
#include "../include/Player.hpp"
#include "../include/Grid.hpp"


void AI::ab_pruning(Grid& grid) const noexcept
{
    int32_t iAlpha = INT_MIN, iBeta = INT_MAX, iDepth = 1, iBestPlay = 0, iMinimaxValue = 0;

    for (uint8_t i = 0; i < Grid::SCyWidth; i++)
    {
        if (grid.isValidPlay(i))
        {
            Grid gridAttempt = grid;
            gridAttempt.makePlay(__EplayerMark, i);
            iMinimaxValue = ab_minValue(gridAttempt, Grid::nextPlayer(__EplayerMark), iDepth + 1, 
                iAlpha, iBeta);

            if (iMinimaxValue > iAlpha)
            {
                iAlpha = iMinimaxValue;
                iBestPlay = i;
            }
        }
    }

    if (grid.isValidPlay(iBestPlay)) 
        grid.makePlay(__EplayerMark, iBestPlay);
}


int32_t AI::ab_minValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int8_t yWinner = Cgrid.checkWinner();

    if (yWinner != Grid::PlayerMark::GRID_TYPE_NONE) return yWinner;
    else if (iDepth == _ySearchLimit) return heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Grid::SCyWidth && iAlpha < iBeta; i++)
        {
            if (Cgrid.isValidPlay(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.makePlay(CEplayerMark, i);
                iBeta = std::min(iBeta, ab_maxValue(gridAttempt, Grid::nextPlayer(CEplayerMark), 
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iBeta;
    }
}


int32_t AI::ab_maxValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int8_t yWinner = Cgrid.checkWinner();

    if (yWinner != Grid::PlayerMark::GRID_TYPE_NONE) return yWinner;
    else if (iDepth == _ySearchLimit) return heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Grid::SCyWidth && iAlpha < iBeta; i++)
        {
            if (Cgrid.isValidPlay(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.makePlay(CEplayerMark, i);
                iAlpha = std::max(iAlpha, ab_minValue(gridAttempt, Grid::nextPlayer(CEplayerMark), 
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iAlpha;
    }
}


int32_t AI::heuristic(const Grid& Cgrid) const noexcept
{
    uint32_t iHeuristic = 0;

    for (uint8_t i = 0; i < Grid::SCyHeight; i++)
    {
        for (uint8_t j = 0; j < Grid::SCyWidth; j++)
        {
            if (Cgrid[i][j] != Grid::PlayerMark::GRID_TYPE_NONE)
            {
                // Vertical up check
                if (i >= Grid::SCyNumberToMatch && 
                    (i == Grid::SCyHeight - 1 || Cgrid[i][j] != Cgrid[i + 1][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && (Cgrid[i - yOffset][j] == Cgrid[i][j] ||
                        Cgrid[i - yOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i - yOffset][j] == Cgrid[i][j]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Vertical down check
                if (i <= Grid::SCyHeight - Grid::SCyNumberToMatch && 
                    (i == 0 || Cgrid[i][j] != Cgrid[i - 1][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && (Cgrid[i + yOffset][j] == Cgrid[i][j] ||
                        Cgrid[i + yOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i + yOffset][j] == Cgrid[i][j]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Horizontal left check
                if (j >= Grid::SCyNumberToMatch && 
                    (j == Grid::SCyWidth - 1 || Cgrid[i][j] != Cgrid[i][j + 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && (Cgrid[i][j - yOffset] == Cgrid[i][j] ||
                        Cgrid[i][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j - yOffset] == Cgrid[i][j]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Horizontal right check
                if (j <= Grid::SCyWidth - Grid::SCyNumberToMatch && 
                    (j == 0 || Cgrid[i][j] != Cgrid[i][j - 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && (Cgrid[i][j + yOffset] == Cgrid[i][j] ||
                        Cgrid[i][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i][j + yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Diagonal up right check
                if (i >= Grid::SCyNumberToMatch && j <= Grid::SCyWidth - Grid::SCyNumberToMatch &&
                    (i == Grid::SCyHeight - 1 || j == 0 || Cgrid[i][j] != Cgrid[i - 1][j - 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && 
                        (Cgrid[i + yOffset][j + yOffset] == Cgrid[i][j] ||
                        Cgrid[i + yOffset][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + yOffset][j + yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Diagonal down left check
                if (i <= Grid::SCyHeight - Grid::SCyNumberToMatch && j >= Grid::SCyNumberToMatch &&
                    (i == 0 || j == Grid::SCyWidth - 1 || Cgrid[i][j] != Cgrid[i - 1][j + 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && 
                        (Cgrid[i + yOffset][j - yOffset] == Cgrid[i][j] ||
                        Cgrid[i + yOffset][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + yOffset][j - yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Diagonal up left check
                if (i >= Grid::SCyNumberToMatch && j >= Grid::SCyNumberToMatch &&
                    (i == Grid::SCyHeight - 1 || j == Grid::SCyWidth - 1 || 
                    Cgrid[i][j] != Cgrid[i + 1][j + 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && 
                        (Cgrid[i - yOffset][j - yOffset] == Cgrid[i][j] ||
                        Cgrid[i - yOffset][j - yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i - yOffset][j - yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
                // Diagonal down right check
                if (i <= Grid::SCyHeight - Grid::SCyNumberToMatch && 
                    j <= Grid::SCyWidth - Grid::SCyNumberToMatch &&
                    (i == 0 || j == 0 || Cgrid[i][j] != Cgrid[i - 1][j - 1]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < Grid::SCyNumberToMatch && (Cgrid[i + yOffset][j + yOffset] == Cgrid[i][j] ||
                        Cgrid[i + yOffset][j + yOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + yOffset][j + yOffset]) yCounter++;
                        yOffset++;
                    }

                    if (yCounter == Grid::SCyNumberToMatch) return Cgrid[i][j];
                    else iHeuristic += (playerMark2Heuristic(Cgrid, i, j) * yCounter);
                }
            }
        }
    }
    return iHeuristic;
}


uint8_t AI::playerMark2Heuristic(const Grid& Cgrid, uint8_t yRow, uint8_t yColumn) const noexcept
{
    if (Cgrid[yRow][yColumn] == __EplayerMark) return 1;
    else return - 1;
}
