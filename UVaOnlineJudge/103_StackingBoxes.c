#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct box
{
    int origin;
    struct box* next_fit;
    int dim[10];
};

struct box boxes_mem[30];
struct box* boxes[30];
int boxCount;
int dimensions;
int thunk[30];

int int_compare(const void* left, const void* right)
{
    const int* left_int = left;
    const int* right_int = right;
    return ((*left_int) - (*right_int));
}

int box_compare(const void* left, const void* right)
{
    struct box* left_box = *((struct box**)left);
    struct box* right_box = *((struct box**)right);
    int result = 0;
    int i;
    for (i = 0; i < dimensions && result == 0; i += 1)
    {
        result = int_compare(&left_box->dim[i], &right_box->dim[i]);
    }
    return result;
}

int box_fits(struct box* inner, struct box* outer)
{
    int i;
    int result = 1;
    for (i = 0; i < dimensions; i += 1)
    {
        /* printf("comparing %d >= %d\n", inner->dim[i], outer->dim[i]); */
        if (inner->dim[i] >= outer->dim[i])
        {
            result = 0;
            break;
        }
    }
    /* printf("comparing %d fits into %d gives %d\n", inner->origin, outer->origin, result); */
    return result;
}

int longest_sequence(int from)
{
    int i;
    int result = 1;
    if (thunk[from] > 0)
    {
        result = thunk[from];
    }
    else
    {
        for (i = from+1; i < boxCount; i += 1)
        {
            if (box_fits(boxes[from], boxes[i]))
            {
                int seqLen = longest_sequence(i) + 1;
                if (seqLen > result)
                {
                    result = seqLen;
                    boxes[from]->next_fit = boxes[i];
                }
            }
        }
        thunk[from] = result;
    }
    return result;
}

int main()
{
    FILE* input = stdin;
    int b,d;
    struct box* iter;
    int seqLen;
    int maxSequence;
    struct box* maxSeqHead;

#ifndef ONLINE_JUDGE
    input = fopen("103_StackingBoxes.txt","r");
    if (NULL == input)
        return 1;
#endif

    while (2 == fscanf(input, "%d %d", &boxCount, &dimensions))
    {
        for (b = 0; b < boxCount; b += 1)
        {
            boxes[b] = &boxes_mem[b];
            boxes[b]->origin = b + 1;
            boxes[b]->next_fit = NULL;
            for (d = 0; d < dimensions; d += 1)
                fscanf(input, "%d", &boxes[b]->dim[d]);
            qsort(boxes[b]->dim, dimensions, sizeof(int), &int_compare);
        }
        qsort(boxes, boxCount, sizeof(boxes[0]), &box_compare);
        maxSequence = 0;
        memset(thunk, 0, sizeof(thunk));
        for (b = 0; b < boxCount; b += 1)
        {
            seqLen = longest_sequence(b);
            if (seqLen > maxSequence)
            {
                maxSequence = seqLen;
                maxSeqHead = boxes[b];
            }
        }
        printf("%d\n", maxSequence);
        for (iter = maxSeqHead; iter != NULL; iter = iter->next_fit)
        {
            printf("%d%c", iter->origin, (iter->next_fit != NULL) ? ' ' : '\n');
        }
    }

    return 0;
}

