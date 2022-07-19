#ifndef GRID_HPP_
#define GRID_HPP_


#include <vector>
#include <ostream>
#include <cstdint>
#include <climits>


class Grid
{
    public:
        enum PlayerMark {GRID_TYPE_NONE = 0, GRID_TYPE_X = INT_MIN, GRID_TYPE_O = INT_MAX};

        uint8_t getHeight() const noexcept;
        uint8_t getWidth() const noexcept;
        int32_t getEmptyCells() const noexcept;
        const std::vector<std::vector<PlayerMark>>& getCells() const noexcept;

        Grid() = default;
        explicit Grid(uint8_t yHeight, uint8_t yWidth);

        static PlayerMark nextPlayer(const PlayerMark& CEPlayerMark) noexcept;

        const std::vector<PlayerMark>& operator [](uint8_t yIndex) const noexcept;
        std::vector<PlayerMark>& operator [](uint8_t yIndex) noexcept;

        void makePlay(const PlayerMark& CEPlayerMark, uint8_t yPlayRow, uint8_t yPlayColumn);
        bool isValidPlay(uint8_t yPlayRow, uint8_t yPlayColumn) const noexcept;
        int32_t endResult() const noexcept;

    private:
        int32_t _yEmptyCells;
        std::vector<std::vector<PlayerMark> > _a2PlayerMarkCells;

};


inline uint8_t Grid::getHeight() const noexcept { return _a2PlayerMarkCells.size(); }
inline uint8_t Grid::getWidth() const noexcept { return _a2PlayerMarkCells[0].size(); }
inline int32_t Grid::getEmptyCells() const noexcept { return _yEmptyCells; }
inline const std::vector<std::vector<Grid::PlayerMark>>& Grid::getCells() const noexcept 
{ return _a2PlayerMarkCells; }

inline const std::vector<Grid::PlayerMark>& Grid::operator [](uint8_t yIndex) const noexcept 
{ return _a2PlayerMarkCells[yIndex]; }
inline std::vector<Grid::PlayerMark>& Grid::operator [](uint8_t yIndex) noexcept 
{ return _a2PlayerMarkCells[yIndex]; }

inline bool operator ==(const Grid& CGrid1, const Grid& CGrid2) noexcept
{ return CGrid1.getCells() == CGrid2.getCells(); }

std::ostream& operator <<(std::ostream& ostream, const Grid::PlayerMark& CEPlayerMark) noexcept;
std::ostream& operator <<(std::ostream& ostream, const Grid& CGrid) noexcept;


#endif