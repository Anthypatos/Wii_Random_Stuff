#include <vector>
#include <string>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include "../include/Grid.hpp"


Grid::Grid(uint8_t yHeight, uint8_t yWidth) : _yEmptyCells{yHeight * yWidth}, 
    _a2PlayerMarkCells{yHeight, std::vector<PlayerMark>{yWidth, GRID_TYPE_NONE}} {}


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
    _yEmptyCells--;
}


bool Grid::isValidPlay(uint8_t yPlayRow, uint8_t yPlayColumn) const noexcept
{ 
    return (_yEmptyCells != 0 && yPlayRow >= 0 && yPlayRow < _a2PlayerMarkCells.size() &&
        yPlayColumn >= 0 && yPlayColumn < _a2PlayerMarkCells[0].size() &&
        _a2PlayerMarkCells[yPlayRow][yPlayColumn] == Grid::PlayerMark::GRID_TYPE_NONE); 
}


int32_t Grid::endResult() const noexcept
{
    return 0; // TO DO
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
