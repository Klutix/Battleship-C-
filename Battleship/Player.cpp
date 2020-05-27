//
// Created by kiren on 4/4/2020.
//

#include "Player.h"

Player::Player(std::string name) {
    Name = name;
}

const std::string &Player::getName() const {
    return Name;
}

void Player::setName(const std::string &name) {
    Name = name;
}

Player::Player() {

}
