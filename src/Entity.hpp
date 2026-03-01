#pragma once

#include "fogpi/Math.hpp"

class Room;

class Entity {
    public:
        Room* room;

        virtual void Start(Vec2 _pos) {}
        virtual void Update() {}

        Vec2 GetPosition() { return m_position; };
        char Draw() { return m_character; }

        void SetPosition(Vec2 pos) { m_position = pos; }
        Vec2 GetPosition() const { return m_position; }

    protected:
        Vec2 m_position;
        char m_character;
};