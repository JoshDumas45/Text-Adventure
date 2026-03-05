#pragma once

#include "Entity.hpp"

class Spike : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update() {}
    void Move();
    void DamagePlayer(int damage);
    int attack = 0;
private:
    int m_direction = 0;
};