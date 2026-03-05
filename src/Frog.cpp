#include "Frog.hpp"
#include "Dice.hpp"
#include "Room.hpp"
#include "fogpi/io.hpp"

void Frog::Start(Vec2 _pos) {

    m_character = 'F';

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

void Frog::Move()
{
    m_frogTime++;

    //  Visible
    if (m_frogTime <= 3)
    {
        m_character = 'F';
        return;
    }

    //  Disappears
    if (m_frogTime <= 5)
    {
        m_character = ' ';
        return;
    }

    // 6: Reappear closer to player
    if (m_frogTime == 6)
    {
        Vec2 playerPos = room->GetPlayerPosition();

        Vec2 direction(0,0);

        if (playerPos.x > m_position.x) direction.x = 1;
        if (playerPos.x < m_position.x) direction.x = -1;
        if (playerPos.y > m_position.y) direction.y = 1;
        if (playerPos.y < m_position.y) direction.y = -1;

        Vec2 tryPos = m_position + direction;

        if (room->GetLocation(tryPos) == ' ')
            m_position = tryPos;

        m_character = 'F';
    }

    // Reset timer
    if (m_frogTime > 6)
        m_frogTime = 0;
}