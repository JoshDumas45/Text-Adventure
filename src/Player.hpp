#pragma once

#include "Entity.hpp"
#include "Goblin.hpp"
#include "Frog.hpp"

class Player : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();

    int health = 0;
    int attack = 3;
    int gold = 0;
    int maxHealth = 0;
    int level = 1;
    int experience = 0;

    void FightGoblin(Goblin* enemy);
    void FightFrog(Frog* enemy);
    void GainXP(int amount);
    void OpenShop();

    bool hasMovedThisTurn = false;

private:
    int m_keyCount = 0;
};