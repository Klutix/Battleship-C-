#ifndef BATTLESHIPOOP_COMPUTERPLAYER_H
#define BATTLESHIPOOP_COMPUTERPLAYER_H

#include "BattleShipPlayer.h"
#include <stack>

class ComputerPlayer : public BattleShipPlayer {
private:
    std::stack<std::string> ShotStack;       ///variable to hold a shotstock for computer
    void GenerateShotStack(std::string pos); ///function to generate shotstack
    std::string GenerateRandomPosition();    ///generates random boarrd position
    bool isValidShot(std::string pos);      ///checks if position is a valid shot
    int TargetOffset;                       //todo
    bool TargetAcquired;                    //todo
    std::string TargetFirstDamageLocation;  //todo
    bool Streak;                            //todo
    int StreakDirectionInt;                 //todo
    bool missedOnStreak;                    //todo
    std::set<int> PreviousStreaks;          //todo
public:
    ComputerPlayer();                       ///constructor
    ComputerPlayer(std::string name);       ///paramterized constructor
    ship shootAt(BattleShipPlayer &player, std::string &pos) override;   ///shoot at player at position
};

#endif //BATTLESHIPOOP_COMPUTERPLAYER_H
