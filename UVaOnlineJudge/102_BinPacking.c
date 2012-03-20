#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct bin
{
    uint32_t brown;
    uint32_t green;
    uint32_t clear;
};

uint32_t movesNeedIfColor(struct bin* bn, char color)
{
    uint32_t result = 0;
    switch (color)
    {
    case 'B':
        result = bn->green + bn->clear;
        break;
    case 'G':
        result = bn->brown + bn->clear;
        break;
    case 'C':
        result = bn->green + bn->brown;
        break;
    }
    return result;
}

const char* bin_combinations[6] =
{
    "BCG",
    "BGC",
    "CBG",
    "CGB",
    "GBC",
    "GCB"
};

int main()
{
    FILE* input = stdin;
    struct bin bins[3];
    int combo;
    uint32_t minMoves;
    const char* minMovesCombo;

#ifndef ONLINE_JUDGE
    input = fopen("102_BinPacking.txt","r");
    if (NULL == input)
        return 1;
#endif

    while (9 == fscanf(input, "%d %d %d %d %d %d %d %d %d",
        &bins[0].brown, &bins[0].green, &bins[0].clear,
        &bins[1].brown, &bins[1].green, &bins[1].clear,
        &bins[2].brown, &bins[2].green, &bins[2].clear))
    {
        minMoves = (uint32_t)-1;
        for (combo = 0; combo < 6; combo += 1)
        {
            uint32_t moves = 0;
            moves += movesNeedIfColor(&bins[0], bin_combinations[combo][0]);
            moves += movesNeedIfColor(&bins[1], bin_combinations[combo][1]);
            moves += movesNeedIfColor(&bins[2], bin_combinations[combo][2]);
            if (moves < minMoves)
            {
                minMoves = moves;
                minMovesCombo = bin_combinations[combo];
            }
        }
        printf("%s %d\n", minMovesCombo, minMoves);
    }

    return 0;
}

