#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h>
#include <iostream>

#include "fogpi/Math.hpp"

#include "Dice.hpp"
#include "Room.hpp"
#include "Entity.hpp"

#include "DataStructures/LinkList.hpp"

int main(int argc, char *argv[])
{
    srand(time(NULL));
// Enable ANSI color support for Windows
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    Room room;
    room.Load("assets/level_1.map");

    while(true)
    {
        room.Update();
    }
    /*LinkList<int> list;
    
    for (int i = 0; i < 5; i++)
        list.Insert(i, i);
    
    int size = list.Count();
    
    for (int i = 0; auto link = list.GetLinkAt(i); i++) {
        std::cout << "link " << link->data << std::endl;
    }

    std::cout << "Hello, Class" << std::endl;

    Vec2 v;
    v.x = 5.0f;

    // random seed
    srand(time(NULL));

    Die die = { .sides = 8 };

    std::vector<Die> dice;
    dice.push_back(die);
    dice.push_back(die);
    dice.push_back(die);

    RollStats stats = RollDice(dice);
    */
    
    return 0;
}
