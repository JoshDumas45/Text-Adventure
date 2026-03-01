#pragma once

#include "Entity.hpp"
#include "Goblin.hpp"

class Player : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();

    int health = 0;
    int attack = 0;
    int gold = 0;

    void Fight(Goblin* enemy);

private:
    int m_keyCount = 0;
};