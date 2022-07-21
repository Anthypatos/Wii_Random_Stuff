#include <vector>
#include <string>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include "../include/Grid.hpp"


Grid::Grid(uint8_t yHeight, uint8_t yWidth, uint8_t yMatchNumber) : _iEmptyCells{yHeight * yWidth}, 
    _yMatchNumber{yMatchNumber},
    _a2PlayerMarkCells{yHeight, std::vector<PlayerMark>{yWidth, GRID_TYPE_NONE}} 
{}


Grid::PlayerMark Grid::nextPlayer(const PlayerMark& CEPlayerMark) noexcept
{ 
    switch(CEPlayerMark)
    {
        case PlayerMark::GRID_TYPE_O:       return PlayerMark::GRID_TYPE_X;     break;
        case PlayerMark::GRID_TYPE_X:       return PlayerMark::GRID_TYPE_O;     break;
        default:                            return PlayerMark::GRID_TYPE_NONE;  break;
    }
}


void Grid::makePlay(const PlayerMark& CEPlayerMark, uint8_t yPlayRow, uint8_t yPlayColumn)
{
    if (!isValidPlay(yPlayRow, yPlayColumn)) throw std::domain_error("Play is not valid");

    _a2PlayerMarkCells[yPlayRow][yPlayColumn] = CEPlayerMark;
    _iEmptyCells--;
}


bool Grid::isValidPlay(uint8_t yPlayRow, uint8_t yPlayColumn) const noexcept
{ 
    return (_iEmptyCells != 0 && yPlayRow >= 0 && yPlayRow < _a2PlayerMarkCells.size() &&
        yPlayColumn >= 0 && yPlayColumn < _a2PlayerMarkCells[0].size() &&
        _a2PlayerMarkCells[yPlayRow][yPlayColumn] == Grid::PlayerMark::GRID_TYPE_NONE); 
}


Grid::PlayerMark Grid::endResult() const noexcept
{
    for (uint8_t i = 0; i < _a2PlayerMarkCells.size(); i++)
    {
        for (uint8_t j = 0; j < _a2PlayerMarkCells[0].size(); j++)
        {
            if (_a2PlayerMarkCells[i][j] != PlayerMark::GRID_TYPE_NONE)
            {
                // Vertical check
                if (i <= _a2PlayerMarkCells.size() - _yMatchNumber &&
                    (i == 0 || _a2PlayerMarkCells[i - 1][j] != _a2PlayerMarkCells[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && _a2PlayerMarkCells[i][j] == _a2PlayerMarkCells[i + yOffset][j])
                        yCounter++, yOffset++;
                    if (yCounter == 4) return _a2PlayerMarkCells[i][j];
                }
                // Horizontal check
                if (j <= _a2PlayerMarkCells[0].size() - _yMatchNumber && 
                    (j == 0 || _a2PlayerMarkCells[i][j - 1] != _a2PlayerMarkCells[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && _a2PlayerMarkCells[i][j] == _a2PlayerMarkCells[i][j + yOffset])
                        yCounter++, yOffset++;
                    if (yCounter == 4) return _a2PlayerMarkCells[i][j];
                }
                // Diagonal left check
                if (i <= _a2PlayerMarkCells.size() - _yMatchNumber && j >= _yMatchNumber &&
                    (i == 0 || j == _a2PlayerMarkCells[0].size() - 1 || 
                    _a2PlayerMarkCells[i - 1][j + 1] != _a2PlayerMarkCells[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && 
                        _a2PlayerMarkCells[i][j] == _a2PlayerMarkCells[i + yOffset][j - yOffset])
                        yCounter++, yOffset++;
                    if (yCounter == 4) return _a2PlayerMarkCells[i][j];
                }
                // Diagonal right check
                if (i <= _a2PlayerMarkCells.size() - _yMatchNumber && 
                    j <= _a2PlayerMarkCells[0].size() - _yMatchNumber &&
                    (i == 0 || j == 0 || _a2PlayerMarkCells[i - 1][j - 1] != _a2PlayerMarkCells[i][j]))
                {
                    uint8_t yOffset = 1, yCounter = 1;
                    while (yCounter < 4 && 
                        _a2PlayerMarkCells[i][j] == _a2PlayerMarkCells[i + yOffset][j + yOffset])
                        yCounter++, yOffset++;
                    if (yCounter == 4) return _a2PlayerMarkCells[i][j];
                }
            }
        }
    }
    return PlayerMark::GRID_TYPE_NONE;
}


std::ostream& operator <<(std::ostream& ostream, const Grid::PlayerMark& CEPlayerMark) noexcept
{
    switch(CEPlayerMark)
    {
        case Grid::PlayerMark::GRID_TYPE_NONE:  return ostream << ' '; break;
        case Grid::PlayerMark::GRID_TYPE_O:     return ostream << 'O'; break;
        case Grid::PlayerMark::GRID_TYPE_X:     return ostream << 'X'; break;
        default:                                return ostream << ' '; break;
    }
}


std::ostream& operator <<(std::ostream& ostream, const Grid& CGrid) noexcept
{
    const std::vector<std::vector<Grid::PlayerMark> > a2PlayerMarkCells = CGrid.getCells();
    std::string sRowSeparator = "";

    for (uint8_t i = 0; i < a2PlayerMarkCells[0].size(); i++) sRowSeparator += "---+";
    sRowSeparator += "---";

    for (uint8_t i = 0; i < a2PlayerMarkCells.size(); i++)
    {
        ostream << sRowSeparator << std::endl;
        for (uint8_t j = 0; j < a2PlayerMarkCells[0].size(); j++)
            ostream << "| " << a2PlayerMarkCells[i][j] << " ";
        ostream << "|" << std::endl;
    }
    ostream << sRowSeparator;

    return ostream;
}
