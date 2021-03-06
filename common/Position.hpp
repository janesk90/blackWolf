#ifndef POSITION_H
#define POSITION_H
#include <vector>
#include <array>
#include "../BlackWolf.hpp"

class Position{
public:

    Position();

    Position (int boardArray[8][8]);

    Position(const Position &givenPos, const Move &move);

    const Unit& operator()(const Square& square) const;


    bool whiteCastleQueen;
    bool whiteCastleKing;
    bool blackCastleQueen;
    bool blackCastleKing;

    bool wasCastle;
    bool wasEnPassant;
    bool wasPromotion;

    void setPromotion(const Square& square, const Unit& chosenPiece);

    Color getTurnColor() const;
    void setTurnColor(Color color);

private:
    Color m_turnColor;

    std::array<std::array<Unit, 8>, 8> m_cells;




    void init();


};

#endif // POSITION_H
