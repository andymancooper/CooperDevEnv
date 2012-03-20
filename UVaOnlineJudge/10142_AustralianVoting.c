#include <stdio.h>
#include <string.h>

#define TRACE() /* printf("%d\n", __LINE__)*/

int main()
{
    int cases = 0;
    FILE* input = stdin;
    
#ifndef ONLINE_JUDGE
    input = fopen("10142_AustralianVoting.txt","r");
#endif
    
    fscanf(input, "%d", &cases);
    
    while (cases > 0)
    {
        int c,round,ballot,selection,maxVotes,winningCandidate,minVotes,rank;
        int candidates = 0;
        int ballots[1000][20];
        int ballotCount = 0;
        char names[20][82];
        char overName[82];
        int votes[20];
        
        TRACE();
        /* printf("starting case %d\n", cases); */
        
        /* read in the candidate names */
        fscanf(input, "%d", &candidates);
        fgets(names[0], 82, input); /* just grab newline */
        memset(names, 0, sizeof(names));
        for (c = 0; c < candidates; c += 1)
        {
            if (c < 20)
                fgets(names[c], 82, input);
            else
                fgets(overName, 82, input);
        }
        if (candidates > 20)
            candidates = 20;
        
        /* read in the ballots */
        memset(ballots, 0, sizeof(ballots));
        while (1)
        {
            char voteLine[1024];
            TRACE();
            memset(voteLine, 0, sizeof(voteLine));
            fgets(voteLine, 1023, input);
            if (voteLine[0] == '\n')
                break;
            
            if (candidates ==
                sscanf(voteLine, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                    &ballots[ballotCount][0],
                    &ballots[ballotCount][1],
                    &ballots[ballotCount][2],
                    &ballots[ballotCount][3],
                    &ballots[ballotCount][4],
                    &ballots[ballotCount][5],
                    &ballots[ballotCount][6],
                    &ballots[ballotCount][7],
                    &ballots[ballotCount][8],
                    &ballots[ballotCount][9],
                    &ballots[ballotCount][10],
                    &ballots[ballotCount][11],
                    &ballots[ballotCount][12],
                    &ballots[ballotCount][13],
                    &ballots[ballotCount][14],
                    &ballots[ballotCount][15],
                    &ballots[ballotCount][16],
                    &ballots[ballotCount][17],
                    &ballots[ballotCount][18],
                    &ballots[ballotCount][19]))
            {
                for (rank = 0; rank < candidates; rank += 1)
                {
                    if (ballots[ballotCount][rank] > candidates)
                    {
                        ballots[ballotCount][rank] = candidates;
                    }
                    else if (ballots[ballotCount][rank] < 1)
                    {
                        ballots[ballotCount][rank] = 0;
                    }
                    ballots[ballotCount][rank] -= 1; /* make them zero-index */
                }
                ballotCount += 1;
            }
            else
            {
                memset(&ballots[ballotCount], 0, sizeof(ballots[ballotCount]));
            }
        }
        
        /* determine the winner */
        for (round = 0; round <= candidates; round += 1)
        {
            TRACE();
            /* printf("starting round %d\n", round); */
            
            /* count votes for each candidate */
            memset(votes, 0, sizeof(votes));
            maxVotes = 0;
            winningCandidate = -1;
            for (ballot = 0; ballot < ballotCount; ballot += 1)
            {
                selection = ballots[ballot][0];
                votes[selection] += 1;
                if (votes[selection] > maxVotes)
                {
                    maxVotes = votes[selection];
                    winningCandidate = selection;
                }
            }
            
            /* detect proper winner */
            if (maxVotes > (ballotCount / 2))
            {
                printf("%s\n", names[winningCandidate]);
                break;
            }
            
            /* detect tying end */
            if (candidates == 0 || maxVotes == (ballotCount / candidates))
            {
                int allAtMax = 1;
                for (c = 0; allAtMax && c < candidates; c += 1)
                {
                   allAtMax = (votes[c] == maxVotes);
                }
                if (allAtMax)
                {
                    for (c = 0; c < candidates; c += 1)
                    {
                        printf("%s", names[c]);
                    }
                    printf("\n");
                    break;
                }
            }
            
            /* need to do another round */
            
            /* find least votes */
            minVotes = 1000;
            for (c = 0; c < candidates; c += 1)
            {
                if (votes[c] < minVotes)
                    minVotes = votes[c];
            }
            /* printf("minVotes: %d\nmaxVotes: %d\n", minVotes, maxVotes); */
            
            /* remove candidates with least votes */
            for (c = 0; c < candidates; c += 1)
            if (votes[c] == minVotes)
            {
                /* printf("eliminating %d (%s)\n", c, names[c]); */
                
                /* one less candidate */
                candidates -= 1;
                
                /* remove that candidate from all ballots */
                for (ballot = 0; ballot < ballotCount; ballot += 1)
                {
                    for (rank = 0; rank < candidates; rank += 1)
                    {
                        if (ballots[ballot][rank] == c)
                        {
                            memmove(&ballots[ballot][rank], &ballots[ballot][rank+1], sizeof(int)*(19-rank));
                            rank -= 1;
                        }
                        else if (ballots[ballot][rank] > c)
                        {
                            ballots[ballot][rank] -= 1;
                        }
                    }
                    ballots[ballot][candidates] = 0;
                }
                
                /* remove that candidate's name from list */
                memmove(&names[c], &names[c+1], 82*(19-c));
                memmove(&votes[c], &votes[c+1], sizeof(int)*(19-c));
                c -= 1;
            }
        }
        
        cases = cases - 1;
    }
    
    return 0;
}