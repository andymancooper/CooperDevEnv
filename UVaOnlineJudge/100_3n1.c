#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_CACHE (4*1024*1024)
uint32_t cycleLen[MAX_CACHE] = { 0, };
uint32_t sequence[1*1024*1024] = { 0, };

int main()
{
    FILE* input = stdin;
    uint32_t i,j,n,start,cycle,maxCycle,backtrack,begin,end;
    
#ifndef ONLINE_JUDGE
    input = fopen("100_3n1.txt","r");
    if (input == NULL)
        return 1;
#endif
    
    cycleLen[1] = 1;
    cycleLen[2] = 2;
    while (2 == fscanf(input, "%u %u", &i, &j))
    {
        maxCycle = 0;
        if (i <= j)
        {
            begin = i;
            end = j;
        }
        else
        {
            begin = j;
            end = i;
        }
        for (start = begin; start <= end; start += 1)
        {
            n = start;
            cycle = 0;
            while ((n >= MAX_CACHE) || (cycleLen[n] == 0))
            {
                /* printf("n: %u\n", n); */
                sequence[cycle] = n;
                cycle += 1;
                
                if (n & 1 == 1)
                {
                    n = n * 3 + 1;
                }
                else
                {
                    n = n / 2;
                }
            }
            backtrack = cycleLen[n];
            while (cycle)
            {
                cycle -= 1;
                backtrack += 1;
                if (sequence[cycle] < MAX_CACHE)
                    cycleLen[sequence[cycle]] = backtrack;
            }
            if (cycleLen[start] > maxCycle)
            {
                maxCycle = cycleLen[start];
            }
        }
        printf("%u %u %u\n", i, j, maxCycle);
    }
    
    return 0;
}