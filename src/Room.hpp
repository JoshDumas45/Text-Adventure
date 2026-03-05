#pragma once

#include <vector>
#include "Door.hpp"
#include "Goblin.hpp"
#include "Frog.hpp"
#include "Spike.hpp"

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
    Vec2 GetPlayerPosition() const;
    void DamagePlayer(int damage);
    bool AreAllEnemiesDead() const;
    bool m_hasChest = false;
    const std::vector<Goblin*>& GetGoblins() const { return m_goblins; }
    const std::vector<Frog*>& GetFrogs() const { return m_frogs; }
    const std::vector<Spike*>& GetSpikes() const { return m_spikes; }

private:
    std::vector<Entity*> m_entities;
    Player* m_player = nullptr;
    //std::vector<Monster*> m_monsters;
    std::vector<std::vector<char>> m_map;
    std::vector<Door> m_doors;
    std::vector<Goblin*> m_goblins;
    std::vector<Frog*> m_frogs;
    std::vector<Spike*> m_spikes;
};