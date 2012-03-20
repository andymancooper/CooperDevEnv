#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>

double conversion[20][20];
int dimensions;

using namespace std;

double evalArbitrage(int curr, vector<int>& exchanges, int& sequenceSize)
{
    if (exchanges.size() == 0)
        return 1.0;
    double rate = conversion[curr][exchanges.front()];
    double testEndRate;
    int sequenceSizeLimit = sequenceSize;
    sequenceSize = 1;
    for (vector<int>::iterator iter = exchanges.begin()+1;
         iter != exchanges.end(); ++iter)
    {
        testEndRate = rate * conversion[*(iter-1)][curr];
        if (testEndRate >= 1.01)
            return testEndRate;
        sequenceSize++;
        if (sequenceSize >= sequenceSizeLimit)
            return 0.0;
        rate *= conversion[*(iter-1)][*iter];
    }
    return rate * conversion[exchanges.back()][curr];
}

int main()
{
    FILE* input = stdin;
    int i,j;
    int exchangeCount;
    double rate;
    int leastExchangeCount;
    double leastExchangeCountRate;
    vector<int> leastExchangeCountSequence;
    
#ifndef ONLINE_JUDGE
    input = fopen("104_Arbitrage.txt","r");
    if (input == NULL)
        return 1;
#endif

    while (1 == fscanf(input, "%d", &dimensions))
    {
        for (i = 0; i < dimensions; i += 1)
        {
            for (j = 0; j < dimensions; j += 1)
            {
                if (j == i)
                {
                    conversion[i][j] = 1.0;
                }
                else
                {
                    fscanf(input, "%lf", &conversion[i][j]);
                }
            }
        }
        vector<int> otherCurrencies;
        rate = 0.0;
        leastExchangeCount = 21;
        leastExchangeCountRate = 0.0;
        leastExchangeCountSequence.clear();
        for (i = 0; i < dimensions; i++)
        {
            otherCurrencies.clear();
            for (j = 0; j < dimensions; j++)
            {
                if (j != i)
                    otherCurrencies.push_back(j);
            }
            do
            {
                exchangeCount = leastExchangeCount;
                rate = evalArbitrage(i, otherCurrencies, exchangeCount);
                if (rate >= 1.01 && exchangeCount < leastExchangeCount)
                {
                    leastExchangeCount = exchangeCount;
                    leastExchangeCountRate = rate;
                    leastExchangeCountSequence.clear();
                    leastExchangeCountSequence.push_back(i);
                    copy(otherCurrencies.begin(),otherCurrencies.begin()+exchangeCount,back_inserter(leastExchangeCountSequence));
                    leastExchangeCountSequence.push_back(i);
                }
            } while (next_permutation(otherCurrencies.begin(), otherCurrencies.end()));
        }
        if (leastExchangeCountRate < 1.01)
        {
            printf("no arbitrage sequence exists\n");
        }
        else
        {
            for (j = 0; j < (leastExchangeCountSequence.size()-1); j++)
                printf("%d ", leastExchangeCountSequence[j] + 1);
            printf("%d\n", leastExchangeCountSequence.back() + 1);
        }
    }
    
    return 0;
}