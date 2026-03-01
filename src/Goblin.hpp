#pragma once
#include "Entity.hpp"
#include <vector>

class Goblin : public Entity
{
    public:
        void Start(Vec2 _pos);

        void MoveTowardsPlayer(Vec2 playerPos, std::vector<std::vector<char>>& map);

        int health = 0;
        int attack = 0;
        int goldReward = 0;

        bool IsDead()
        {
            return health <= 0;
        }
};