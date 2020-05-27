//
// Created by kiren on 4/4/2020.
//

#ifndef BATTLESHIPOOP_GAME_H
#define BATTLESHIPOOP_GAME_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>

#include "BattleShipPlayer.h"
#include "ComputerPlayer.h"

using namespace std;

class Game {

public:
    Game();
    void start();
private:
    bool shootFromTo(BattleShipPlayer &player, BattleShipPlayer &other);
    void KeyToContinue();
    vector<ship> GenerateEnemyShips();
    void FillShips(int sz, string pos, char orientation, ship &sh);
    string GenerateRandomPosition();
    const bool ValidateShot(string pos);
    bool advance(int player, bool &alive);   //advance game forward
    vector<ship> FileToShips(ifstream &file);
    bool vCheckDown(shipType sz, std::string pos, std::map<std::string,bool> &board );
    bool vCheckRight(shipType sz, std::string pos, std::map<std::string,bool> &board );
    bool validateCSV(std::ifstream &file);
    bool ValidateEntry(shipType type, std::string pos, char orientation,std::map<std::string,bool> &board); //maybe lambda

    string GetTorpedoDamageResult(ship  &sh);

    void sm(mode m);

    void print(const wchar_t *c, bool tabNewL);
    void print(std::string str, bool tabNewL);
    void printNumRow();
    void clearScreen();
    void DrawBoard(BattleShipPlayer player, BattleShipPlayer other, bool quit = false);
};


#endif //BATTLESHIPOOP_GAME_H
