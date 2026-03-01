#include "Goblin.hpp"
#include "Dice.hpp"
#include "Room.hpp"

void Goblin::Start(Vec2 _pos)
{
    m_character = 'G';
    m_position = _pos;

    // Health: Roll 2 6 sided dice + 5
    std::vector<Die> healthDice = { {6}, {6} };
    RollStats healthStats = RollDice(healthDice);
    health = healthStats.total + 5;

    // Gold Reward: Roll 1 6 sided dice + 1
    std::vector<Die> goldDice = { {6} };
    RollStats goldStats = RollDice(goldDice);
    goldReward = goldStats.total + 1;
}

void Goblin::MoveTowardsPlayer(Vec2 playerPos, std::vector<std::vector<char>>& map)
{
    Vec2 tryPos = m_position;

    if (m_position.x < playerPos.x)
        tryPos.x += 1;
    else if (m_position.x > playerPos.x)
        tryPos.x -= 1;
    else if (m_position.y < playerPos.y)   
        tryPos.y += 1;
    else if (m_position.y > playerPos.y)
        tryPos.y -= 1;

    if (tryPos.y >= 0 && tryPos.y < map.size() &&
        tryPos.x >= 0 && tryPos.x < map[0].size())
    {
        char tile = map[tryPos.y][tryPos.x];

        if (tile != '#' && tile != 'D' && tile != 'L')
        {
            m_position = tryPos;
        }
    }
}