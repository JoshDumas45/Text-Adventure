#pragma once

#include "fogpi/io.hpp"
#include <vector>

struct Die
{
    int sides = 20;
};

struct RollStats
{
    int min = 0;
    int max = 0;
    int greatest = 0;
    int critCount = 0;
    int total = 0;
};

inline RollStats RollDice(std::vector<Die> _dice)
{
    RollStats stats = {};

    for (Die d : _dice)
    {
        stats.min += 1;
        stats.max += d.sides;

        int roll = random_int(1, d.sides);//1 + rand() % d.sides;

        stats.total += roll;

        if (roll > stats.greatest)
            stats.greatest = roll;
        
        if (roll == d.sides)
            stats.critCount++;
    }

    return stats;
}

