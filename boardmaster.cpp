#include "boardmaster.h"
#include <sstream>
#include <boost/assert.hpp>
#include <boost/bimap/support/lambda.hpp>

bool boardMaster::pieceHeld()
{
    return (currentPiece!=pieces.right.end());
}

void boardMaster::releasePiece()
{
    currentPiece = pieces.right.end();
}

void boardMaster::handleCastle(const int row, const int col)
{
    if (row==0){
        if (col==2){
            cellsNpieces::right_iterator rookSprite = pieces.project_right(pieces.left.find(squareId(0,0)));
            pieces.right.modify_data(rookSprite, boost::bimaps::_data = squareId(0,3));
            pieces.right.modify_key(rookSprite, boost::bimaps::_key =
                    changePosition(rookSprite->first,sf::Vector2f(rectGrid[0][3].left,rectGrid[0][3].top)));
        }else{
            BOOST_ASSERT_MSG(col==6, "Invalid Castle");
            cellsNpieces::right_iterator rookSprite = pieces.project_right(pieces.left.find(squareId(0,7)));
            pieces.right.modify_data(rookSprite, boost::bimaps::_data = squareId(0,5));
            pieces.right.modify_key(rookSprite, boost::bimaps::_key =
                    changePosition(rookSprite->first,sf::Vector2f(rectGrid[0][5].left,rectGrid[0][5].top)));
        }
    }else{
        if (col==2){
            BOOST_ASSERT_MSG(row==7, "Invalid Castle");
            cellsNpieces::right_iterator rookSprite = pieces.project_right(pieces.left.find(squareId(7,0)));
            pieces.right.modify_data(rookSprite, boost::bimaps::_data = squareId(7,3));
            pieces.right.modify_key(rookSprite, boost::bimaps::_key =
                    changePosition(rookSprite->first,sf::Vector2f(rectGrid[7][3].left,rectGrid[7][3].top)));
        }else{
            BOOST_ASSERT_MSG(col==6, "Invalid Castle");
            cellsNpieces::right_iterator rookSprite = pieces.project_right(pieces.left.find(squareId(7,7)));
            pieces.right.modify_data(rookSprite, boost::bimaps::_data = squareId(7,5));
            pieces.right.modify_key(rookSprite, boost::bimaps::_key =
                    changePosition(rookSprite->first,sf::Vector2f(rectGrid[7][5].left,rectGrid[7][5].top)));
        }
    }
}

void boardMaster::handleEnPassant(const int row, const int col)
{
    if (row==5){
        destroy(4,col);
    }else{
        BOOST_ASSERT_MSG(row==2, "Invalid en passant");
        destroy(3,col);
    }
}

void boardMaster::destroy(const int row, const int col)
{
    squareId toDelete(row,col);

    pieces.left.erase(toDelete);

}

boardMaster::boardMaster(sf::Window &theWindow):
    flipOffset(0,0),
    window_(sfg::Canvas::Create()),
    bigWindow(theWindow),
    turnLabel_(sfg::Label::Create("White to play")),
    whiteClockCanvas_(sfg::Canvas::Create()),
    blackClockCanvas_(sfg::Canvas::Create()),
    moveList(sfg::Table::Create()),
    plyCounter(0)
{
    releasePiece();

    moveList->SetColumnSpacings(10.f);

    window_->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    window_->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(&boardMaster::processLeftClick, this);
    window_->GetSignal(sfg::Widget::OnMouseMove).Connect(&boardMaster::processMouseMove, this);
    window_->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(&boardMaster::processMouseRelease, this);
    window_->GetSignal(sfg::Widget::OnMouseEnter).Connect(&boardMaster::processEnterCanvas, this);

    font.loadFromFile("DejaVuSans.ttf"); //assert it


    boardTexture_.loadFromFile("Graphics/Boardbrown.jpg");
    boardSprite_.setTexture(boardTexture_);    

    blackRookT.loadFromFile("Graphics/Pieces/BlackR.png");
    blackBishopT.loadFromFile("Graphics/Pieces/BlackB.png");
    blackKnightT.loadFromFile("Graphics/Pieces/BlackN.png");
    blackQueenT.loadFromFile("Graphics/Pieces/BlackQ.png");
    blackKingT.loadFromFile("Graphics/Pieces/BlackK.png");
    blackPawnT.loadFromFile("Graphics/Pieces/BlackP.png");
    whiteRookT.loadFromFile("Graphics/Pieces/WhiteR.png");
    whiteBishopT.loadFromFile("Graphics/Pieces/WhiteB.png");
    whiteKnightT.loadFromFile("Graphics/Pieces/WhiteN.png");
    whiteQueenT.loadFromFile("Graphics/Pieces/WhiteQ.png");
    whiteKingT.loadFromFile("Graphics/Pieces/WhiteK.png");
    whitePawnT.loadFromFile("Graphics/Pieces/WhiteP.png");

    int idCount = 1;

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const int pieceId = currentPosition[i][j];
            if (pieceId==0) continue;
            //const sf::Texture &pieceTexture = idToTexture(pieceId);
            squareId cellId(i,j);
            pieceSprite toAdd(idToTexture(pieceId),cellToPosition(i,j),pieceId, idCount);
            pieces.insert(cellsNpieces::value_type(cellId,toAdd));
            idCount++;
        }
    }

    rectGrid.resize(8);
    for (int i=0; i<8; ++i){
        rectGrid[i].resize(8);
        for (int j=0; j<8; ++j){
            sf::Vector2f toSet = cellToPosition(i,j);
            rectGrid[i][j].left = toSet.x;
            rectGrid[i][j].top = toSet.y;
            rectGrid[i][j].width = 50.f;
            rectGrid[i][j].height = 50.f;
        }
    }

    whiteClockText.setFont(font);
    whiteClockText.setCharacterSize(20);
    //whiteClockText.setPosition(0.f, 0.f);
    whiteClockText.setColor(sf::Color(0, 140, 190));

    blackClockText.setFont(font);
    blackClockText.setCharacterSize(20);
    //blackClockText.setPosition(70.f, 150.f);
    blackClockText.setColor(sf::Color(0, 140, 190));

    whiteClockCanvas_->SetRequisition(sf::Vector2f(100,50));
    blackClockCanvas_ = sfg::Canvas::Create();
    blackClockCanvas_->SetRequisition(sf::Vector2f(100,50));

    whiteClock.restart(sf::seconds(300));
    blackClock.restart(sf::seconds(300));
    blackClock.stop();


    updateClocks();





}

void boardMaster::display()
{
    window_->Clear();

    window_->Draw(boardSprite_);

    for (auto &piece : pieces){
        window_->Draw(piece.right);
    }
    updateClocks();

    //window_->Display();

}

sf::Vector2f boardMaster::cellToPosition(const int row, const int col) const
{
    return sf::Vector2f(flipOffset.x * (9 - 2 * col) + 20 + 50 * col, (flipOffset.y * (9 - 2 * row)) + 420 - 50 * (row+1));
}

const sf::Texture &boardMaster::idToTexture(const int pieceId) const
{
    switch (pieceId) {
    case 1:
        return whiteRookT;
    case 2:
        return whiteBishopT;
    case 3:
        return whiteKnightT;
    case 4:
        return whiteQueenT;
    case 5:
        return whitePawnT;
    case 6:
        return whiteKingT;
    case -1:
        return blackRookT;
    case -2:
        return blackBishopT;
    case -3:
        return blackKnightT;
    case -4:
        return blackQueenT;
    case -5:
        return blackPawnT;
    case -6:
        return blackKingT;
    }
}

sf::Vector2f boardMaster::getMousePosition()
{
    sf::Vector2f windowPos = window_->GetAbsolutePosition() + static_cast<sf::Vector2f>(bigWindow.getPosition());
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - windowPos);
}

int boardMaster::getTurnColor() const
{
    return currentPosition.turnColor;
}

void boardMaster::switchTurn()
{
    if (getTurnColor() == 1){
        blackClock.stop();
        whiteClock.start();
        turnLabel_->SetText("White to play");
    }else{
        whiteClock.stop();
        blackClock.start();
        turnLabel_->SetText("Black to play");
    }
}

void boardMaster::sendBack()
{
    BOOST_ASSERT_MSG(pieceHeld(), "No current piece to send back");

    pieces.right.modify_key(currentPiece, boost::bimaps::_key =
            changePosition(currentPiece->first,cellToPosition(currentPiece->second.row, currentPiece->second.col)));

    //currentPiece->setPosition(cellToPosition(currentPiece->row,currentPiece->col));
    releasePiece();
}

void boardMaster::processLeftClick()
{
    clickedPoint = getMousePosition();

    const int whoseTurn = getTurnColor();

    for (auto &piece : pieces){
        if (piece.right.contains(clickedPoint)){
            if (piece.right.getSide()!=whoseTurn) return;
            currentPiece = pieces.right.find(piece.right);
            break;
        }
    }
}

void boardMaster::processMouseMove()
{
    if (pieceHeld()){
        pieces.right.modify_key(currentPiece, boost::bimaps::_key =
                changePosition(currentPiece->first,getMousePosition()-sf::Vector2f(25.f,25.f)));
    }
}

void boardMaster::processMouseRelease()
{
    if (pieceHeld()){
        sf::Vector2f centrePos = currentPiece->first.getPosition() + sf::Vector2f(25.f,25.f);
        for (int i=0; i<8; ++i){
            for (int j=0; j<8; ++j){
                if (rectGrid[i][j].contains(centrePos)){
                    const int originRow = currentPiece->second.row;
                    const int originCol = currentPiece->second.col;
                    completeMove toCheck(currentPosition,originRow,originCol,i,j);
                    if (toCheck.isLegal()){
                        destroy(i,j);
                        pieces.right.modify_data(currentPiece, boost::bimaps::_data = squareId(i,j));
                        pieces.right.modify_key(currentPiece, boost::bimaps::_key =
                                changePosition(currentPiece->first,sf::Vector2f(rectGrid[i][j].left,rectGrid[i][j].top)));

                        releasePiece();
                        currentPosition = toCheck.getNewBoard();
                        if (currentPosition.wasCastle) handleCastle(i,j);
                        if (currentPosition.wasEnPassant) handleEnPassant(i,j);
                        switchTurn();
                        sfg::Label::Ptr newMove(sfg::Label::Create(moveToString(originRow,originCol,i,j)));
                        const int plyPairsCount = plyCounter/2;
                        const int plyRemainder = (plyCounter)%2;
                        moveList->Attach(newMove,{plyRemainder,plyPairsCount,1,1});
                        plyCounter++;
                    }else{
                        sendBack();
                    }
                    return;
                }
            }
        }
        sendBack();
    }
}

void boardMaster::processEnterCanvas()
{
    if (pieceHeld()){
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) sendBack();
    }
}

pieceSprite boardMaster::changePosition(pieceSprite piece, const sf::Vector2f position) const
{
    piece.setPosition(position);
    return piece;
}

std::string boardMaster::toString(sf::Time value) const
{
    int minutes = value.asSeconds()/60;
    int seconds = static_cast<int>(value.asSeconds())%60;

    std::ostringstream stream;
    stream.setf(std::ios_base::fixed);
    stream.precision(2);
    stream << minutes << ":" << seconds;
    return stream.str();
}

std::string boardMaster::colToString(const int col) const
{
    switch (col) {
    case 0:
        return "a";
    case 1:
        return "b";
    case 2:
        return "c";
    case 3:
        return "d";
    case 4:
        return "e";
    case 5:
        return "f";
    case 6:
        return "g";
    case 7:
        return "h";
    }
}

std::string boardMaster::moveToString(const int row1, const int col1, const int row2, const int col2) const
{
    return (cellToString(row1,col1) + "-" + cellToString(row2,col2));
}

std::string boardMaster::cellToString(const int row, const int col) const
{
    return (colToString(col) + std::to_string(row+1));
}

void boardMaster::updateClocks()
{
    whiteClockText.setString(toString(whiteClock.getRemainingTime()));
    whiteClockCanvas_->Clear();
    whiteClockCanvas_->Draw(whiteClockText);

    blackClockText.setString(toString(blackClock.getRemainingTime()));
    blackClockCanvas_->Clear();
    blackClockCanvas_->Draw(blackClockText);    
}
