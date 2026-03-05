#include "Room.hpp"

#include "Player.hpp"
#include "Dice.hpp"
#include <fstream>
#include <string>

void Room::Load(std::string _path)
{
    m_map.clear();
    m_doors.clear();
    m_goblins.clear();
    m_spikes.clear();
    
    Vec2 playerSpawn;
    bool foundSpawn = false;

    std::ifstream file;
    file.open(_path);

    if (!file.is_open())
    {
        printf("file not found at: %s \n", _path.c_str());
        exit(1);
    }

    std::string word;
    int number;

    while (file >> word)
    {
        if (word == "level")
        {
            if (file >> number)
            {
                printf("open level: %i\n", number);
            }
        }

        if (word == "next_level")
        {
            if (file >> word)
            {
                m_doors.push_back(Door());
                m_doors[m_doors.size() - 1].path = word;
            }
        }

        if (word == "map")
        {
            m_map.push_back(std::vector<char>());
            while(file >> word)
            {
                if (word == "-2")
                {
                    break;
                }

                if (word == "-1")
                {
                    m_map.push_back(std::vector<char>());
                    continue;
                }

                if (word == "0")
                    m_map[m_map.size() - 1].push_back(' ');
                else
                    m_map[m_map.size() - 1].push_back(word[0]);
                
            }
        }
    }

    int doorCount = 0;

    for (int y = 0; y < m_map.size(); y++)
    {
        for (int x = 0; x < m_map[y].size(); x++)
        {
            if (m_map[y][x] == 'S')
            {
                playerSpawn = Vec2(x,y);
                foundSpawn = true;
                
                if (m_player == nullptr)
                {
                    m_player = new Player();
                    m_player->Start(Vec2(x,y));
                    m_player->room = this;
                }
                else
                {
                    m_player->SetPosition(Vec2(x,y));
                }
                m_map[y][x] = ' ';
            }

            if (m_map[y][x] == 'D' || m_map[y][x] == 'L')
            {
                if (m_doors.size() - 1 >= doorCount)
                {
                    m_doors[doorCount].pos.x = x;
                    m_doors[doorCount].pos.y = y;
                    doorCount++;
                }
            }
        }
    }

    // Spawns goblins
    int goblinsToSpawn = 1;
    for (int i = 0; i < goblinsToSpawn; i++)
    {
        while (true)
        {
            int x = rand() % m_map[0].size();
            int y = rand() % m_map.size();

            if (m_map[y][x] == ' ' && Vec2(x, y) != playerSpawn)
            {
                Goblin* g = new Goblin();
                g->Start(Vec2(x, y));
                g->room = this;
                m_goblins.push_back(g);
                break;
            }
        }
    }

    int frogsToSpawn = 1;
    for (int i = 0; i < frogsToSpawn; i++)
    {
        while (true)
        {
            int x = rand() % m_map[0].size();
            int y = rand() % m_map.size();

            if (m_map[y][x] == ' ' && Vec2(x, y) != playerSpawn)
            {
                Frog* f = new Frog();
                f->room = this;
                f->Start(Vec2(x, y));
                m_frogs.push_back(f);
                break;
            }
        }
    }

    int spikesToSpawn = 1;
    for (int i = 0; i < spikesToSpawn; i++)
    {
        while (true)
        {
            int x = rand() % m_map[0].size();
            int y = rand() % m_map.size();

            if (m_map[y][x] == ' ' && Vec2(x, y) != playerSpawn)
            {
                Spike* s = new Spike();
                s->room = this;
                s->Start(Vec2(x, y));
                m_spikes.push_back(s);
                break;
            }
        }
    }

    // 30% chance of treasure chest room
    if (random_int(1, 100) <= 30)
    {
        m_hasChest = true;

        while (true)
        {
            int x = rand() % m_map[0].size();
            int y = rand() % m_map.size();

            if (m_map[y][x] == ' ')
            {
                m_map[y][x] = 'C'; // C = chest
                break;
            }
        }
    }
    else
    {
        m_hasChest = false;
    }
}

Vec2 Room::GetPlayerPosition() const
{
    if (m_player != nullptr)
        return m_player->GetPosition();

    return Vec2(0,0);
}

bool Room::AreAllEnemiesDead() const
{
    for (auto g : m_goblins)
    {
        if (!g->IsDead())
            return false;
    }
    for (auto f : m_frogs)
    {
        if (!f->IsDead())
            return false;
    }
    return true;
}

void Room::DamagePlayer(int damage)
{
    if (m_player != nullptr)
    {
        m_player->health -= damage;

        printf("You were hit by spikes! -%d HP\n", damage);

        if (m_player->health <= 0)
        {
            printf("You died! Gold collected: %d\n", m_player->gold);
            exit(0);
        }
    }
}

void Room::Update()
{
    Draw();

    if (m_player != nullptr)
    {
        m_player->room = this;
        m_player->Update();

        if (m_player->hasMovedThisTurn)
        {
            for (auto f : m_frogs)
            {
                f->Move();
            }
            for (auto s : m_spikes)
            {
                s->Move();
            }
            for (auto g : m_goblins)
            {
                g->MoveTowardsPlayer(m_player->GetPosition(), m_map);
                if (g->GetPosition() == m_player->GetPosition())
                {
                    m_player->FightGoblin(g);
                }
            }
        }
    }
}

void Room::Draw()
{
    for (int y = 0; y < m_map.size(); y++)
    {
        for (int x = 0; x < m_map[y].size(); x++)
        {
            // Print map characters in different colors based on what they represent
            /* ANSI Color Codes
            Black = 30
            Red = 31
            Green = 32
            Yellow = 33
            Blue = 34
            Magenta = 35
            Cyan = 36
            White = 37
            */
            char c = GetLocation(Vec2(x, y));

            for (auto g : m_goblins)
            {
                if (g->GetPosition() == Vec2(x, y))
                {
                    c = g->Draw();
                    break;
                }
            }
            for (auto f : m_frogs)
            {
                if (f->GetPosition() == Vec2(x, y))
                {
                    c = f->Draw();
                    break;
                }
            }
            for (auto s : m_spikes)
            {
                if (s->GetPosition() == Vec2(x, y))
                {
                    c = s->Draw();
                    break;
                }
            }

            //printf("%c ", c);

            switch(c)
            {
                case '#': printf("\033[1;34m %c\033[0m", c); break;
                case 'P': printf("\033[1;32m %c\033[0m", c); break;
                case 'F': printf("\033[1;31m %c\033[0m", c); break;
                case '^': printf("\033[1;31m %c\033[0m", c); break;
                case 'G': printf("\033[1;31m %c\033[0m", c); break;
                case 'K': printf("\033[1;33m %c\033[0m", c); break;
                case 'D': printf("\033[1;35m %c\033[0m", c); break;
                case 'L': printf("\033[1;37m %c\033[0m", c); break;
                case 'C': printf("\033[1;33m %c\033[0m", c); break;
                default:  printf("%c ", c); break;
            }
        }
        printf("\n");
    }
}

char Room::GetLocation(Vec2 _pos)
{
    if (_pos.y >= m_map.size())
        return ' ';
    
    if (_pos.x >= m_map[_pos.y].size())
        return ' ';

    if (m_player != nullptr)
        if (m_player->GetPosition() == _pos)
            return m_player->Draw();
    
    return m_map[_pos.y][_pos.x];
}

void Room::ClearLocation(Vec2 _pos)
{
    if (_pos.y >= m_map.size())
        return;
    
    if (_pos.x >= m_map[_pos.y].size())
        return;
    
    m_map[_pos.y][_pos.x] = ' ';
}

void Room::OpenDoor(Vec2 _pos)
{
    if (!AreAllEnemiesDead())
    {
        printf("The door is locked defeat all enemies first.\n");
        return;
    }

    for(int i = 0; i < m_doors.size(); i++)
    {
        if (m_doors[i].pos == _pos)
        {
            if (m_player != nullptr)
            {
                m_player->OpenShop();
            }

            Load(m_doors[i].path.c_str());
            // 40% chance to heal between rooms
            if (random_int(1,100) <= 40)
            {
                int healAmount = m_player->maxHealth / 4;
                m_player->health += healAmount;

                if (m_player->health > m_player->maxHealth)
                    m_player->health = m_player->maxHealth;

                printf("You feel rested. Healed %d HP.\n", healAmount);
            }
            return;
        }
    }
}