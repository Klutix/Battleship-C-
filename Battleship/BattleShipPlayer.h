//
// Created by kiren on 4/4/2020.
//

#ifndef BATTLESHIPOOP_BATTLESHIPPLAYER_H
#define BATTLESHIPOOP_BATTLESHIPPLAYER_H

#include "Player.h"
#include "ship.h"
#include "iomanip"

class BattleShipPlayer : public Player {
public:
    BattleShipPlayer();
    BattleShipPlayer(std::string name);

    std::vector<ship> &getShips() ;

    std::set<std::string> & getUsedShots() ;

    void setUsedShots(const std::set<std::string> &usedShots);

     std::set<std::string> &getMissedShots() ;

    void setMissedShots(const std::set<std::string> &missedShots);

    void setShips(const std::vector<ship> &ships);

    const bool isAlive() override;
    const bool isUsedShot(std::string pos);


    virtual ship shootAt(BattleShipPlayer &player,std::string &pos);
private:
    std::set<std::string> usedShots;
    std::vector<ship> ships;          //starts the game
protected:
    std::set<std::string> missedShots;
};


#endif //BATTLESHIPOOP_BATTLESHIPPLAYER_H
