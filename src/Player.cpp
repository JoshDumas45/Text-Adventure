#include "Player.hpp"
#include "Room.hpp"
#include "Dice.hpp"
#include "fogpi/io.hpp"

void Player::Start(Vec2 _pos) {
    m_character = 'P';
    m_position = _pos;

    // Health: Roll 3 6 sided dice + 8
    std::vector<Die> healthDice = { {6}, {6}, {6} };
    RollStats healthStats = RollDice(healthDice);
    maxHealth = healthStats.total + 8;
    health = maxHealth;
}

void Player::GainXP(int amount)
{
    experience += amount;

    int xpToLevel = level * 10;

    if (experience >= xpToLevel)
    {
        experience -= xpToLevel;
        level++;

        maxHealth += 5;
        attack += 1;
        health = maxHealth;

        printf("\n!!!LEVEL UP!!! You are now level %d\n", level);
        printf("Max HP and Attack increased\n\n");
    }
}

void Player::Update() {
    //while(request_char("hit w to continue: ") != 'w') {}
    // Show player stats
    printf("PLAYER  HP: %d/%d  ATTACK: %d  GOLD: %d\n", health, maxHealth, attack, gold);
    printf("LEVEL: %d  XP: %d\n", level, experience);

    char directionInput;

    do {
        directionInput = request_char("wasd and Enter to move");
    } while (directionInput != 'w' &&
             directionInput != 'a' &&
             directionInput != 's' &&
             directionInput != 'd');
    
    Vec2 direction(0.0f);

    switch (directionInput)
    {
    case 'w':
        direction = {0.0f, -1.0f};
        break;
    case 'a':
        direction = {-1.0f, 0.0f};
        break;
    case 's':
        direction = {0.0f, 1.0f};
        break;
    case 'd':
        direction = {1.0f, 0.0f};
        break;
    default:
        direction = {0.0f, 1.0f};
        break;
    }

    Vec2 tryPos = m_position + direction;

    if (room->GetLocation(tryPos) == 'C')
    {
        printf("You found a treasure chest!\n");

        gold += random_int(5, 15);
        printf("You gained gold!\n");

        if (random_int(1,100) <= 50)
        {
            printf("You found a key inside!\n");
            m_keyCount++;
        }

        room->ClearLocation(tryPos);
    }

    // fight detection
    for (auto g : room->GetGoblins())
    {
        if (g->GetPosition() == tryPos)
        {
            FightGoblin(g);
            return;
        }
    }
    for (auto f : room->GetFrogs())
    {
        if (f->GetPosition() == tryPos)
        {
            FightFrog(f);
            return;
        }
    }
    for (auto s : room->GetSpikes())
    {
        if (s->GetPosition() == tryPos)
        {
            health -= 3;
            return;
        }
    }

    if (room->GetLocation(tryPos) == 'K') {
        m_keyCount++;
        room->ClearLocation(tryPos);
    } 

    if (room->GetLocation(tryPos) == ' ') {
        m_position = tryPos;
        hasMovedThisTurn = true; // Player actually moved
    } 
    else 
    {
        hasMovedThisTurn = false; // Player didn't move
    }

    if (room->GetLocation(tryPos) == 'D') {
        room->OpenDoor(tryPos);
    }
}

// Fight system for goblin
void Player::FightGoblin(Goblin* enemy)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("Goblin Fight\n\n");
    while (health > 0 && enemy->health > 0)
    {
        // Display stats
        printf("PLAYER  HP: %d/%d  ATTACK: %d  GOLD: %d\n", health, maxHealth, attack, gold);;
        printf("ENEMY   HP: %d     ATTACK: 1\n", enemy->health);

        char input = request_char("Press 'a' to attack");
        if (input == 'a')
        {   
            // Attack: Rolls 1 6 sided dice + 3
            std::vector<Die> playerDice = { {6} };
            RollStats playerStats = RollDice(playerDice);
            int playerDamage = playerStats.total + attack;
            enemy->health -= playerDamage;

            printf("\nYou rolled a %d and hit the enemy for %d damage!\n", playerStats.total, playerDamage);

            if (enemy->health > 0)
            {
                // Attack: Rolls 1 4 sided dice + 1
                std::vector<Die> enemyDice = { {4} };
                RollStats enemyStats = RollDice(enemyDice);
                int enemyDamage = enemyStats.total + 1;
                health -= enemyDamage;

                printf("Enemy rolled a %d and hit you for %d damage!\n\n", enemyStats.total, enemyDamage);
            }
        }
        else
        {
            printf("\nWrong button press 'a' to attack\n");
        }
    }

    // Fight ended
    if (health > 0)
    {
        printf("You defeated the enemy! You gained %d gold!\n", enemy->goldReward);
        gold += enemy->goldReward;
        GainXP(5);
        enemy->SetPosition(Vec2(-1, -1)); // remove from map
    }
    else
    {
        printf("You died! Gold collected: %d\n", gold);
        exit(0);
    }

    request_char("Press Enter to continue...");
}

// Fight system for Frogs
void Player::FightFrog(Frog* enemy)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("Frog Fight\n\n");
    while (health > 0 && enemy->health > 0)
    {
        // Display stats
        printf("PLAYER  HP: %d/%d  ATTACK: %d  GOLD: %d\n", health, maxHealth, attack, gold);
        printf("ENEMY   HP: %d     ATTACK: 2\n", enemy->health);

        char input = request_char("Press 'a' to attack");
        if (input == 'a')
        {   
            // Attack: Rolls 1 6 sided dice + 3
            std::vector<Die> playerDice = { {6} };
            RollStats playerStats = RollDice(playerDice);
            int playerDamage = playerStats.total + attack;
            enemy->health -= playerDamage;

            printf("\nYou rolled a %d and hit the enemy for %d damage!\n", playerStats.total, playerDamage);

            if (enemy->health > 0)
            {
                // Attack: Rolls 1 4 sided dice + 2
                std::vector<Die> enemyDice = { {4} };
                RollStats enemyStats = RollDice(enemyDice);
                int enemyDamage = enemyStats.total + 2;
                health -= enemyDamage;

                printf("Enemy rolled a %d and hit you for %d damage!\n\n", enemyStats.total, enemyDamage);
            }
        }
        else
        {
            printf("\nWrong button press 'a' to attack\n");
        }
    }

    // Fight ended
    if (health > 0)
    {
        printf("You defeated the enemy! You gained %d gold!\n", enemy->goldReward);
        gold += enemy->goldReward;
        GainXP(5);
        enemy->SetPosition(Vec2(-1, -1)); // remove from map
    }
    else
    {
        printf("You died! Gold collected: %d\n", gold);
        exit(0);
    }

    request_char("Press Enter to continue...");
}

void Player::OpenShop()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    char choice;

    do
    {
        printf("SHOP\n");
        printf("HP: %d/%d\n", health, maxHealth);
        printf("Gold: %d\n\n", gold);

        printf("1) +5 Max Health (10 gold)\n");
        printf("2) Increase Attack (15 gold)(current attack %d)\n", attack);
        printf("3) Heal 10 HP (5 gold)\n");
        printf("4) Continue\n");

        choice = request_char("> ");

        switch (choice)
        {
            case '1':
                if (gold >= 10)
                {
                    gold -= 10;
                    maxHealth += 5;
                    health += 5;
                    printf("Max health increased!\n");
                }
                else printf("Not enough gold!\n");
                break;

            case '2':
                if (gold >= 15)
                {
                    gold -= 15;
                    attack += 2;
                    printf("Attack increased!\n");
                }
                else printf("Not enough gold!\n");
                break;

            case '3':
                if (gold >= 5)
                {
                    gold -= 5;
                    health += 10;

                    if (health > maxHealth)
                        health = maxHealth;

                    printf("You healed 10 HP!\n");
                }
                else printf("Not enough gold!\n");
                break;
        }

    } while (choice != '4');
}