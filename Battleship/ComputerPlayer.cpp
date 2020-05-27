//
// Created by kiren on 4/4/2020.
//

#include "ComputerPlayer.h"


///checks if shot is valid
bool ComputerPlayer::isValidShot(std::string pos) {
    if(std::isdigit(pos[1]) && !isUsedShot(pos))
        return (pos[0] >= 'A' && pos[0] <= 'J' && pos[1] > '0' && std::stoi(pos.substr(1)) <= 10);
    return false;
}

///Generates a random position in battleship in relation to Battleship Gameboard
std::string ComputerPlayer::GenerateRandomPosition(){
    char c;
    int r;
    std::string pos= "";
    // initialize the random number generator
    r = rand() % 10 ;   // generate a random numberA1
    c = 'A' + r;       // Convert to a character from a-z

    pos+=c;
    r = rand() % 10 + 1 ;   // generate a random number
    c= '0'+ r;
    if(r != 10){
        pos+=c;
    } else{
        pos+="10";
    }
    return pos;

}


///generates a shotstack for the computer to use if a succesful shot was previously made
void ComputerPlayer::GenerateShotStack(std::string pos) {
    char l = pos[0];
    char n = pos[1];

//    if(!Streak) {
        n = n - TargetOffset; //up
        std::string up = std::string(1, l) + n;
        n = n + TargetOffset; //down
        std::string down = std::string(1, l) + n;
        n = pos[1]; //reset n
        l = l - TargetOffset;    //left
        std::string left = std::string(1, l) + n;
        l = l + TargetOffset;    //right
        std::string right = std::string(1, l) + n;

        ShotStack.push(up);
        ShotStack.push(down);
        ShotStack.push(left);
        ShotStack.push(right);

//    }else{
////        switch(StreakDirectionInt){
////            case 0:
////                for(int i = 1 ; i <= 4; i++){
////                    l = l + i; //right
////                    std::string right = std::string(1, l) + n;
////                        ShotStack.push(right);
////                }
////                break;
////            case 1:
////                for(int i = 1 ; i <= 4; i++){
////                    l = l - i; //left
////                    std::string left = std::string(1, l) + n;
////                        ShotStack.push(left);
////                }
////                break;
////            case 2:
////                for(int i = 1 ; i <= 4; i++){
////                    n = n + i; //down
////                    std::string down = std::string(1, l) + n;
////                        ShotStack.push(down);
////                }
////                break;
////            case 3:
////                for(int i = 1 ; i <= 4; i++){
////                    n = n - i; //up
////                    std::string up = std::string(1, l) + n;
////                        ShotStack.push(up);
////                }
////                break;
////        }
///    }
}

///shoots at a player at pos
ship ComputerPlayer::shootAt(BattleShipPlayer &player, std::string &pos) {
    redo:
//    if(Streak){
//        missedOnStreak = true;
//        pos = TargetFirstDamageLocation;
//    }

    if(ShotStack.size() > 0) {
        pos = ShotStack.top();
        ShotStack.pop();
    }

    if (isValidShot(pos)) {
        getUsedShots().insert(pos);
    }else{
        if(ShotStack.size() > 0){
            goto redo;
        }else{
            pos = GenerateRandomPosition();
            goto redo;
        }
    }
    ship ResultSh;
    for (auto &sh : player.getShips()) {
        if (sh.status) {
            if (sh.DamagePositionMap.find(pos) != sh.DamagePositionMap.end()) {
                sh.DamagePositionMap[pos] = true;
//                if(TargetAcquired && !Streak){
//                    StreakDirectionInt = ShotStack.size();
//                    Streak = true;
//                }else{
//                    TargetFirstDamageLocation = pos;
//                    TargetAcquired = true;
//                }

                //generate shot stack from pos
                GenerateShotStack(pos);
                if (!sh.isAlive()) {
                    sh.status = false;
                    ShotStack = std::stack<std::string>(); //this should clear the stack
                    ResultSh = sh;
                  //  TargetAcquired = false;
                  //  Streak = false;
                 //   PreviousStreaks.clear();
                    return ResultSh;
                } else {
                    /*
                    if(Streak)
                        missedOnStreak = true;
                    StreakDirectionInt = ShotStack.size();
                    switch (StreakDirectionInt){
                        case 0:
                            PreviousStreaks.insert(0);
                            if(PreviousStreaks.count(1))
                                StreakDirectionInt = 2;
                            else
                                StreakDirectionInt = 1;
                            break;
                        case 1:
                            PreviousStreaks.insert(1);
                            if(PreviousStreaks.count(0))
                                StreakDirectionInt = 2;
                            else
                                StreakDirectionInt = 0;
                            break;
                        case 2:
                            PreviousStreaks.insert(2);
                            if(PreviousStreaks.count(3))
                                StreakDirectionInt = 0;
                            else
                                StreakDirectionInt = 3;
                            break;
                        case 3:
                            PreviousStreaks.insert(3);
                            if(PreviousStreaks.count(2))
                                StreakDirectionInt = 1;
                            else
                                StreakDirectionInt = 2;
                            break;
                    }
                     */
                    ResultSh = sh;
                    return ResultSh;
                }
            }
        }
    }
    missedShots.insert(pos);
    return ResultSh;
}
///constructor
ComputerPlayer::ComputerPlayer() {
    ShotStack.push("");
    ShotStack.pop();
    //TargetAcquired = false;
    TargetOffset = 1;
}
///paramatized constructor
ComputerPlayer::ComputerPlayer(std::string name) : BattleShipPlayer(name) {
        ShotStack.push("");
        ShotStack.pop();
    //TargetAcquired = false;
    TargetOffset = 1;
}
