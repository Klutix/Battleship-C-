#include "BattleShipPlayer.h"

const bool BattleShipPlayer::isAlive() {
        bool alive = false;
        for(auto e : ships)
            if(e.status)
                alive = true;
        return alive;
}

const bool BattleShipPlayer::isUsedShot(std::string pos) {
        return usedShots.count(pos);
}

BattleShipPlayer::BattleShipPlayer(std::string name) : Player(name) {
    Name =name;
}

 std::vector<ship>& BattleShipPlayer::getShips()  {
    return ships;
}

void BattleShipPlayer::setShips(const std::vector<ship> &ships) {
    BattleShipPlayer::ships = ships;
}

BattleShipPlayer::BattleShipPlayer() {}

 std::set<std::string>& BattleShipPlayer::getUsedShots()  {
    return usedShots;
}

void BattleShipPlayer::setUsedShots(const std::set<std::string> &usedShots) {
    BattleShipPlayer::usedShots = usedShots;
}

 std::set<std::string> &BattleShipPlayer::getMissedShots()  {
    return missedShots;
}

void BattleShipPlayer::setMissedShots(const std::set<std::string> &missedShots) {
    BattleShipPlayer::missedShots = missedShots;
}

ship BattleShipPlayer::shootAt(BattleShipPlayer &player,std::string &pos) {

    if (!isUsedShot(pos)) {
        getUsedShots().insert(pos);
    }
    ship ResultSh;
    for (auto &sh : player.getShips()) {
        if (sh.status) {
            if (sh.DamagePositionMap.find(pos) != sh.DamagePositionMap.end()) {
                sh.DamagePositionMap[pos] = true;
                if (!sh.isAlive()) {
                    sh.status = false;
                    ResultSh = sh;
                    return ResultSh;
                } else {
                    ResultSh = sh;
                    return ResultSh;
                }
            }
        }
    }
    missedShots.insert(pos);
    return ResultSh;
}




