#include "boardmove.h"
#include <boost/math/special_functions/sign.hpp>
#include <boost/assert.hpp>
#include <cmath>

boardMove::boardMove(const position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2):
    board(thePosition),
    row1(theRow1),
    col1(theCol1),
    row2(theRow2),
    col2(theCol2),
    newBoard(thePosition,theRow1,theCol1,theRow2,theCol2)
{
    //BOOST_ASSERT_MSG((row1!=row2)||(col1!=col2), "Start and end square are the same");


    pieceCode = board[row1][col1];
    BOOST_ASSERT_MSG(pieceCode != 0, "No piece in starting square");
    BOOST_ASSERT_MSG(pieceCode*board.turnColor>0, "Trying to move opponent's piece");
}

bool boardMove::isLegal() const
{
    if (startEndSame()) return false;

    if (isOccupied()) return false;

    switch (std::abs(pieceCode)) {
    case 1:
        return isRookLegal();
    case 2:
        return isBishopLegal();
    case 3:
        return isKnightLegal();
    case 4:
        return isQueenLegal();
    case 5:
        return isPawnLegal();
    case 6:
        return isKingLegal();
    }
}


bool boardMove::startEndSame() const
{
    return ((row1==row2)&&(col1==col2));
}

bool boardMove::isOccupied() const
{
    //checks if occupied by piece of same color
    return (pieceCode*board[row2][col2]>0);
}

bool boardMove::isKnightLegal() const
{
    for (int i=-2; i<3; i++){
        for (int j=-2; j<3; j++){
            if ((i!=0)&&(j!=0)&&(std::abs(i) != std::abs(j))){
                if ((row1+i == row2)&&(col1+j == col2)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool boardMove::isBishopLegal() const
{

    const int deltaRow = row2 - row1;
    const int deltaCol = col2 - col1;

    //if not in same diagonal, do not proceed
    if (std::abs(deltaRow) != std::abs(deltaCol)) return false;

    const int signRowDiff = boost::math::sign(deltaRow);
    const int signColDiff = boost::math::sign(deltaCol);

    for (int i=row1+signRowDiff; i!=row2; i+=signRowDiff){
        int j = col1 + (i-row1)*signColDiff*signRowDiff;
        if (board[i][j] != 0) return false;
    }
    return true;
}

bool boardMove::isRookLegal() const
{
    if (row1==row2){
        const int signDiff = boost::math::sign(col2 - col1);
        const int maxIt = col2 - col1 - signDiff;

        for (int i=signDiff; i<=maxIt; i+=signDiff){
            if (board[row1][col1+i] != 0) return false;
        }
        return true; //no obstruction
    }else if (col1==col2){
        const int signDiff = boost::math::sign(row2 - row1);
        const int maxIt = row2 - row1 - signDiff;

        for (int i=signDiff; i<=maxIt; i+=signDiff){
            if (board[row1+i][col1] != 0) return false;
        }
        return true; //no obstruction
    }
    return false; //not in same row nor column
}

bool boardMove::isQueenLegal() const
{
    return (isBishopLegal() || isRookLegal());
}

bool boardMove::isKingLegal() const
{
    if (newBoard.wasCastle) return true; //tentative true
    //completeMove will ultimately decide if the castle is legal

    if ((std::abs(row2-row1)>1) || (std::abs(col2-col1)>1)) return false;

    return isQueenLegal();
}

bool boardMove::isPawnLegal() const
{
    //can be rewritten much smaller

    if (board[row1][col1]>0){ //white pawn
        if ((row1==1)&&(row2==3)&&(col1==col2)){ //double advance
            if ((board[row2][col2]==0)&&(board[2][col2]==0))
                return true;
        }else{
            if (row1+1==row2){
                if (col1==col2)
                    if (board[row2][col2]==0) return true;
                if ((col1+1==col2)||(col1-1==col2))
                    if (pieceCode*board[row2][col2]<0) return true;
            }
        }
    }else{
        if ((row1==6)&&(row2==4)&&(col1==col2)){ //double advance
            if ((board[row2][col2]==0)&&(board[5][col2]==0))
                return true;
        }else{
            if (row1-1==row2){
                if (col1==col2)
                    if (board[row2][col2]==0) return true;
                if ((col1+1==col2)||(col1-1==col2))
                    if (pieceCode*board[row2][col2]<0) return true;
            }
        }
    }
    return false;
}

bool boardMove::isObstructed(const int pieceCode) const
{
    return ((pieceCode!=0)&&(pieceCode!=7));
}
