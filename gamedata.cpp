#include "gamedata.h"

gameData::gameData()
{
    whiteClock.connect(std::bind(&gameData::setResult, this, bw::Black));
    blackClock.connect(std::bind(&gameData::setResult, this, bw::White));
}

bool gameData::ended() const
{
    return (result != bw::None);
}

bool gameData::userTurn() const
{
    return (turnColor & userColor);
}

void gameData::setResult(const bw winner)
{
    result = winner;
}

void gameData::newGame(const bw whoUser)
{
    plyCounter = 0;

    turnColor = bw::White;

    userColor = whoUser;

    result = bw::None;

    whiteClock.restart(sf::seconds(300.f));
    blackClock.restart(sf::seconds(300.f));
    blackClock.stop();

    //update
}

void gameData::update()
{
    whiteClock.update();
    blackClock.update();
}

void gameData::switchTurn()
{
    if (turnColor == bw::White){
        blackClock.stop();
        whiteClock.start();
    }else{
        BOOST_ASSERT_MSG(turnColor==bw::Black, "Turn color invalid");
        whiteClock.stop();
        blackClock.start();
    }
}