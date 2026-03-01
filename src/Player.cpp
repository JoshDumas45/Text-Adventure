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

void Player::Update() {
    //while(request_char("hit w to continue: ") != 'w') {}
    // Show player stats
    printf("HP: %d/%d  GOLD: %d\n", health, maxHealth, gold);

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

    // fight detection
    for (auto g : room->GetGoblins())
    {
        if (g->GetPosition() == tryPos)
        {
            Fight(g);
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
void Player::Fight(Goblin* enemy)
{
    while (health > 0 && enemy->health > 0)
    {
        // Display stats
        printf("PLAYER  HP: %d/%d  GOLD: %d\n", health, maxHealth, gold);
        printf("ENEMY   HP: %d\n", enemy->health);

        char input = request_char("Press 'a' to attack");
        if (input == 'a')
        {   
            // Attack: Rolls 1 6 sided dice + 3
            std::vector<Die> playerDice = { {6} };
            RollStats playerStats = RollDice(playerDice);
            int playerDamage = playerStats.total + 3 + attack;
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
        printf("2) Increase Attack (15 gold)(current attack bonus +%d)\n", attack);
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