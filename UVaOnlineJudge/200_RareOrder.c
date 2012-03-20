#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lessThan[26][26] = { { 0, } };
int used[26];

int main()
{
    FILE* input = stdin;
    char* curr_line;
    char* last_line;
    char* temp_line;
    int k,i,j;
    int iter,usedCount;
    char line_one_mem[22] = { '\0', };
    char line_two_mem[22] = { '\0', };
    char result[27] = "                          ";
    
#ifndef ONLINE_JUDGE
    input = fopen("200_RareOrder.txt","r");
    if (input == NULL)
        return 1;
#endif
    
    curr_line = &line_one_mem[0];
    last_line = &line_two_mem[0];
    
    fgets(curr_line, 22, input);
    while (curr_line[0] != '#')
    {
        /* clear whitespace from end of line */
        for (i = strlen(curr_line)-1; i >= 0 && !isalpha(curr_line[i]); i -= 1)
            curr_line[i] = '\0';
        
        /* apply used */
        for (i = 0; curr_line[i]; i += 1)
            used[curr_line[i] - 'A'] = 1;
        
        /* find the first letters which are different */
        for (i = 0; curr_line[i] == last_line[i] && curr_line[i] != '\0'; i += 1);
        if (curr_line[i] != '\0' && last_line[i] != '\0') /* there is order information */
        {
            int pre_idx = last_line[i] - 'A';
            int post_idx = curr_line[i] - 'A';
            /* printf("order information is %c comes after %c\n", curr_line[i], last_line[i]); */
            
            lessThan[pre_idx][post_idx] = 1;
        }
        
        /* read the next line */
        temp_line = curr_line;
        curr_line = last_line;
        last_line = temp_line;
        fgets(curr_line, 22, input);
    }
    
    /* fill in the rest of the table */
    for (k = 0; k < 26; k += 1)
    if (used[k])
    for (i = 0; i < 26; i += 1)
    if (used[i])
    for (j = 0; j < 26; j += 1)
    if (used[j])
    {
        if (lessThan[i][k] && lessThan[k][j])
        {
            lessThan[i][j] = 1;
        }
    }

    /* fill result list */
    usedCount = 0;
    for (i = 0; i < 26; i += 1)
        if (used[i])
            usedCount += 1;
    for (i = 0; i < 26; i += 1)
    if (used[i])
    {
        k = 1;
        for (j = 0; j < 26; j += 1)
            if (lessThan[i][j])
                k += 1;
        result[usedCount-k] = 'A' + i;
    }
    
    /*printf("  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z\n");
    for (i = 0; i < 26; i += 1)
    {
        printf("%c ", 'A' + i);
        for (j = 0; j < 26; j += 1)
            printf("%d ", lessThan[i][j]);
        printf("\n");
    }

    printf("  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z\n");
    for (i = 0; i < 26; i += 1)
    {
        printf("%c ", 'A' + i);
        for (j = 0; j < 26; j += 1)
            printf("%c ", next[i][j] + 'A');
        printf("\n");
    }*/
    
    printf("%s\n", result);

    return 0;
}
