#include "Spike.hpp"
#include "Room.hpp"
#include "fogpi/io.hpp"

void Spike::Start(Vec2 _pos) {
    m_character = '^';
    m_position = _pos;
}
void Spike::Move() {
    
    if (m_direction == 0)
    {
        Vec2 direction(0.0f);

        direction = {0.0f, 1.0f};
            //Test Position try
        Vec2 tryPos = m_position + direction;

        if (room->GetLocation(tryPos) == ' ') {
            m_direction = 1;
        }
        else{
            m_position = tryPos;
        }
        
    }
    if (m_direction == 1)
    {
        Vec2 direction(0.0f);

        direction = {0.0f, -1.0f};
            //Test Position try
        Vec2 tryPos = m_position + direction;

        if (room->GetLocation(tryPos) == ' ') {
            m_direction = 0;
        }
        else{
            m_position = tryPos;
        }
    }
}