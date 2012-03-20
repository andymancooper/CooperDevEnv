#include <stdio.h>
#include <string.h>

#define TRACE() /* printf("%d\n", __LINE__)*/

#define MAX_CANDIDATES 20
#define MAX_BALLOTS 1000
#define MAX_NAME_LEN (80+2) /* add two for newline and null */

struct candidate
{
    char name[MAX_NAME_LEN];
    int votes;
    struct candidate* next;
};

struct ballot_entry
{
    struct candidate* selection;
    struct ballot_entry* next;
};

struct candidate candidates[MAX_CANDIDATES];
struct ballot_entry ballots[MAX_BALLOTS][MAX_CANDIDATES];

struct candidate candidate_list_end;
struct ballot_entry ballot_list_end;

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
        int c,round,ballot,selection,maxVotes,minVotes,rank;
        int candidateCount = 0;
        int ballotCount = 0;
        char overName[MAX_NAME_LEN];
        struct candidate* eligibleCandidates = &candidates[0];
        struct candidate* winningCandidate;
        struct ballot_entry* ballotHead[MAX_BALLOTS];
        
        TRACE();
        /* printf("starting case %d\n", cases); */
        
        /* read in the candidate names */
        fscanf(input, "%d", &candidateCount);
        fgets(overName, sizeof(overName), input); /* just grab newline */
        memset(candidates, 0, sizeof(candidates));
        for (c = 0; c < candidateCount; c += 1)
        {
            if (c < MAX_CANDIDATES)
            {
                fgets(candidates[c].name, MAX_NAME_LEN, input);
                if (c < (MAX_CANDIDATES-1))
                    candidates[c].next = &candidates[c+1];
            }
            else
            {
                fgets(overName, sizeof(overName), input);
            }
        }
        if (candidateCount == 0)
        {
            eligibleCandidates = &candidate_list_end;
        }
        else
        {
            if (candidateCount > 20)
                candidateCount = 20;
            candidates[candidateCount-1].next = &candidate_list_end;
        }
        
        /* read in the ballots */
        memset(ballots, 0, sizeof(ballots));
        while (1)
        {
            char voteLine[1024];
            int selections[MAX_CANDIDATES];
            TRACE();
            memset(voteLine, 0, sizeof(voteLine));
            if (NULL == fgets(voteLine, 1023, input))
                break;
            if (voteLine[0] == '\n')
                break;
            
            memset(selections, 0, sizeof(selections));
            if (candidateCount ==
                sscanf(voteLine, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                    &selections[0],
                    &selections[1],
                    &selections[2],
                    &selections[3],
                    &selections[4],
                    &selections[5],
                    &selections[6],
                    &selections[7],
                    &selections[8],
                    &selections[9],
                    &selections[10],
                    &selections[11],
                    &selections[12],
                    &selections[13],
                    &selections[14],
                    &selections[15],
                    &selections[16],
                    &selections[17],
                    &selections[18],
                    &selections[19]))
            {
                for (rank = 0; rank < candidateCount; rank += 1)
                {
                    if (selections[rank] > candidateCount)
                    {
                        ballots[ballotCount][rank].selection = &candidates[candidateCount-1];
                    }
                    else if (selections[rank] < 1)
                    {
                        ballots[ballotCount][rank].selection = &candidates[0];
                    }
                    else
                    {
                        ballots[ballotCount][rank].selection = &candidates[selections[rank]-1];
                    }
                    if (rank < (candidateCount-1))
                    {
                        ballots[ballotCount][rank].next = &ballots[ballotCount][rank+1];
                    }
                    else
                    {
                        ballots[ballotCount][rank].next = &ballot_list_end;
                    }
                }
                ballotHead[ballotCount] = &ballots[ballotCount][0];
                ballotCount += 1;
            }
        }
        
        /* determine the winner */
        for (round = 0; round <= MAX_CANDIDATES; round += 1)
        {
            struct candidate** c_iter;
            struct ballot_entry** b_iter;
            
            TRACE();
            /* printf("starting round %d\n", round); */
            
            /* count votes for each candidate */
            for (c_iter = &eligibleCandidates; *c_iter != &candidate_list_end; c_iter = &((*c_iter)->next))
                (*c_iter)->votes = 0;
            maxVotes = 0;
            winningCandidate = NULL;
            for (ballot = 0; ballot < ballotCount; ballot += 1)
            {
                ballotHead[ballot]->selection->votes += 1;
                if (ballotHead[ballot]->selection->votes > maxVotes)
                {
                    maxVotes = ballotHead[ballot]->selection->votes;
                    winningCandidate = ballotHead[ballot]->selection;
                }
            }
            
            /* detect proper winner */
            if (maxVotes > (ballotCount / 2))
            {
                printf("%s\n", winningCandidate->name);
                break;
            }
            
            /* detect tying end */
            if (candidateCount == 0 || maxVotes == (ballotCount / candidateCount))
            {
                int allAtMax = 1;
                for (c_iter = &eligibleCandidates; allAtMax && *c_iter != &candidate_list_end; c_iter = &((*c_iter)->next))
                {
                   allAtMax = ((*c_iter)->votes == maxVotes);
                }
                if (allAtMax)
                {
                    for (c_iter = &eligibleCandidates; *c_iter != &candidate_list_end; c_iter = &((*c_iter)->next))
                    {
                        printf("%s", (*c_iter)->name);
                    }
                    printf("\n");
                    break;
                }
            }
            
            /* need to do another round */
            
            /* find least votes */
            minVotes = 1000;
            for (c_iter = &eligibleCandidates; *c_iter != &candidate_list_end; c_iter = &((*c_iter)->next))
            {
                if ((*c_iter)->votes < minVotes)
                    minVotes = (*c_iter)->votes;
            }
            /* printf("minVotes: %d\nmaxVotes: %d\n", minVotes, maxVotes); */
            
            /* remove candidates with least votes */
            for (c_iter = &eligibleCandidates; *c_iter != &candidate_list_end;)
            if ((*c_iter)->votes == minVotes)
            {
                /* printf("eliminating %s\n", (*c_iter)->name); */
                
                /* one less candidate */
                candidateCount -= 1;
                
                /* remove that candidate from all ballots */
                for (ballot = 0; ballot < ballotCount; ballot += 1)
                {
                    for (b_iter = &ballotHead[ballot]; *b_iter != &ballot_list_end;)
                    {
                        if ((*b_iter)->selection == *c_iter)
                        {
                            *b_iter = (*b_iter)->next;
                        }
                        else
                        {
                            b_iter = &((*b_iter)->next);
                        }
                    }
                }
                
                /* remove that candidate's name from list */
                *c_iter = (*c_iter)->next;
            }
            else
            {
                c_iter = &((*c_iter)->next);
            }
        }
        
        cases = cases - 1;
    }
    
    return 0;
}