//
// Created by kiren on 4/4/2020.
//

#ifndef BATTLESHIPOOP_PLAYER_H
#define BATTLESHIPOOP_PLAYER_H

#include <iostream>
#include <set>
#include <vector>
class Player {
public:
    Player();
    Player(std::string Name);
    virtual const bool isAlive() = 0;

    const std::string &getName() const;
    void setName(const std::string &name);
protected:
    std::string Name;

};


#endif //BATTLESHIPOOP_PLAYER_H
