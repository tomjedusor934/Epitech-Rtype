/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Player
*/

#include "Player.hpp"

rtype::teamContainer::Player::Player()
{
}

rtype::teamContainer::Player::Player(int id, profile_t *profile, bool isReady)
    : _id(id), _isReady(isReady), _profile(profile)
{
}

rtype::teamContainer::Player::~Player()
{
}
