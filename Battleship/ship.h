//
// Created by kiren on 4/4/2020.
//

#ifndef BATTLESHIPOOP_SHIP_H
#define BATTLESHIPOOP_SHIP_H

#include <iostream>
#include <map>
enum shipType {
    Carrier = 5, Battleship = 4, Submarine = 3, Destroyer = 3, Cruiser = 2
};

static std::map<int, std::string> EnumMap = {{5 , "Carrier"},{4 , "Battleship"},{ 3 , "Submarine"},{ 3 , "Destroyer"}, {2 , "Cruiser"  }};

enum mode{text = 0x4000, U16 =0x20000 };

class ship {


public:
    std::string type;                         //ship type
    std::map<std::string, bool> DamagePositionMap;    //damage map of each ship position
    bool status;

    const bool isAlive();
private:

};



#endif //BATTLESHIPOOP_SHIP_H
