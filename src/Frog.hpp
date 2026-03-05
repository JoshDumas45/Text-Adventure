#pragma once

#include "Entity.hpp"

class Frog : public Entity
{
public:
  void Start(Vec2 _pos);
  void Update() {}

  int health = 0;
  int attack = 0;
  int goldReward = 0;

  void Move();
  
  bool IsDead()
  {
    return health <= 0;
  }

  int m_frogTime = 0;
};

