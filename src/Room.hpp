#pragma once

#include <vector>
#include "Door.hpp"
#include "Goblin.hpp"

class Entity;
class Player;
class Monster;

class Room {
public:
    void Load(std::string _path);
    void Draw();
    void Update();
    char GetLocation(Vec2 _pos);
    void ClearLocation(Vec2 _pos);
    void OpenDoor(Vec2 _pos);
    const std::vector<Goblin*>& GetGoblins() const { return m_goblins; }

private:
    std::vector<Entity*> m_entities;
    Player* m_player = nullptr;
    //std::vector<Monster*> m_monsters;
    std::vector<std::vector<char>> m_map;
    std::vector<Door> m_doors;
    std::vector<Goblin*> m_goblins;
};