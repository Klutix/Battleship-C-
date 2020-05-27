//
// Created by kiren on 4/4/2020.
//

#include "ship.h"

const bool ship::isAlive() {
        bool alive = false;
        for(auto e : DamagePositionMap)
            if(!e.second)
                alive = true;
        return alive;
}
