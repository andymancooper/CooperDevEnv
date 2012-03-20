#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct block
{
    int origin;
    struct block* prev;
    struct block* next;
};

#define MAX_BLOCKS 25

struct block blocks[MAX_BLOCKS];
struct block places[MAX_BLOCKS];
struct block stack_end[MAX_BLOCKS];

struct block* find_place(struct block* blk)
{
    while (blk->origin >= 0)
        blk = blk->prev;
    return blk;
}

struct block* find_top(struct block* blk)
{
    while (blk->origin >= 0)
        blk = blk->next;
    return blk;
}

void remove_block(struct block* blk)
{
    blk->prev->next = blk->next;
    blk->next->prev = blk->prev;
    blk->next = NULL;
    blk->prev = NULL;
}

void insert_before(struct block* blk, struct block* aft)
{
    blk->next = aft;
    blk->prev = aft->prev;
    aft->prev = blk;
    blk->prev->next = blk;
}

void insert_after(struct block* blk, struct block* bef)
{
    blk->next = bef->next;
    blk->prev = bef;
    bef->next = blk;
    blk->next->prev = blk;
}

void clear_top(struct block* blk)
{
    struct block* iter;
    for (iter = blk->next; iter->origin >= 0;)
    {
        struct block* moving = iter;
        iter = iter->next;
        remove_block(moving);
        insert_after(moving,&places[moving->origin]);
    }
}

int main()
{
    FILE* input = stdin;
    int worldSize = 0;
    char cmd[5],adj[5];
    int src,dst,i;
    struct block* iter;
    struct block* top;
    
#ifndef ONLINE_JUDGE
    input = fopen("101_Blocks2.txt","r");
    if (input == NULL)
        return 1;
#endif
    
    /* initialize the blocks and places*/
    for (i = 0; i < MAX_BLOCKS; i += 1)
    {
        places[i].origin = -1;
        places[i].prev = NULL;
        places[i].next = &blocks[i];
        blocks[i].origin = i;
        blocks[i].prev = &places[i];
        blocks[i].next = &stack_end[i];
        stack_end[i].origin = -1;
        stack_end[i].next = NULL;
        stack_end[i].prev = &blocks[i];
    }
    
    fscanf(input, "%d", &worldSize);
    while (4 == fscanf(input, "%s %d %s %d", cmd, &src, adj, &dst))
    {
        int operation = 0;
        if (src == dst || find_place(&blocks[src]) == find_place(&blocks[dst]))
            continue;
        if (0 == strcmp(cmd,"move"))
        {
            operation = 0x10;
        }
        else if (0 == strcmp(cmd, "pile"))
        {
            operation = 0x20;
        }
        if (0 == strcmp(adj, "onto"))
        {
            operation = operation | 0x1;
        }
        else if (0 == strcmp(adj, "over"))
        {
            operation = operation | 0x2;
        }
        switch (operation)
        {
            case 0x11: /* move onto */
                clear_top(&blocks[src]);
                clear_top(&blocks[dst]);
                remove_block(&blocks[src]);
                insert_after(&blocks[src],&blocks[dst]);
                break;
            case 0x12: /* move over */
                clear_top(&blocks[src]);
                remove_block(&blocks[src]);
                insert_before(&blocks[src],find_top(&blocks[dst]));
                break;
            case 0x21: /* pile onto */
                clear_top(&blocks[dst]);
                for (iter = &blocks[src]; iter->origin >= 0;)
                {
                    struct block* moving = iter;
                    iter = iter->next;
                    remove_block(moving);
                    insert_after(moving, &blocks[dst]);
                }
                break;
            case 0x22: /* pile over */
                top = find_top(&blocks[dst]);
                for (iter = &blocks[src]; iter->origin >= 0;)
                {
                    struct block* moving = iter;
                    iter = iter->next;
                    remove_block(moving);
                    insert_before(moving, top);
                }
                break;
            default:
                break;
        }
    }
    
    for (i = 0; i < worldSize; i += 1)
    {
        printf("%d:", i);
        for (iter = places[i].next; iter->origin >= 0; iter = iter->next)
            printf(" %d", iter->origin);
        printf("\n");
    }
    
    return 0;
}