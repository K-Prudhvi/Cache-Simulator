#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
int main()
{
    FILE *file;
    char filename[100];
    char line[100];
    file = fopen("cache.config", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    int i = 0;
    int j = 0;
    int arr[3];
    char str[2][9];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        if (i < 3)
        {
            arr[i] = atoi(line);
        }
        else
        {
            strcpy(str[j], line);
            j++;
        }
        i++;
    }
    fclose(file);
    int sets;
    if(arr[2]==0){
        sets=1;
        arr[2]=arr[0]/arr[1];
    }
    else{
    sets = arr[0] / (arr[1] * arr[2]);
    }
    int index_bits = log2(sets);
    int offset = log2(arr[1]);
    int tag = 32 - (index_bits + offset);
    int cache[sets][arr[2]];
    for (int i = 0; i < sets; i++)
    {
        for (int j = 0; j < arr[2]; j++)
        {
            cache[i][j] = -1;
        }
    }
    file = fopen("cache.access", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    char line1[50];
    char hex[100];
    char hex1[20];
    char hex2[20];
    int c = 0;
    unsigned int decimal;
    int hitcount = 0;
    int misscount = 0;
    char *y;
    while (fgets(line1, sizeof(line1), file) != NULL)
    {
        line1[strcspn(line1, "\n")] = '\0';
        char *token = strtok(line1, ": ");
        y = strdup("");
        strcpy(y, token);
        while (token != NULL)
        {
            decimal = strtol(token, NULL, 16);
            c++;
            token = strtok(NULL, ": ");
        }
        sprintf(hex, "%x", decimal);
        int t = decimal >> (index_bits + offset);
        int k = pow(2, tag) - 1;
        t = t & k;
        sprintf(hex2, "%x", t);
        int s = (decimal >> offset);
        k = pow(2, index_bits) - 1;
        s = s & k;
        sprintf(hex1, "%x", s);
        int h = 0;
        if (strcmp(str[0], "FIFO") == 0)
        {
            for (int i = 0; i < arr[2]; i++)
            {
                if (cache[s][i] == t)
                {
                     printf("Address: 0x%s, Set: 0x%s ,Hit, Tag: 0x%s\n",hex,hex1,hex2);
                    h = 1;
                    hitcount++;
                    break;
                }
                else if (cache[s][i] == -1)
                {
                     printf("Address: 0x%s, Set: 0x%s ,Miss,Tag: 0x%s\n",hex,hex1,hex2);
                    if ((strcmp(y, "W") == 0 && strcmp(str[1], "WB") == 0) || strcmp(y, "R") == 0)
                    {
                        cache[s][i] = t;
                    }
                    misscount++;
                    h = 1;
                    break;
                }
            }
            if (h == 0)
            {
                 printf("Address: 0x%s, Set: 0x%s ,Miss,Tag: 0x%s\n",hex,hex1,hex2);
                int i;
                if ((strcmp(y, "W") == 0 && strcmp(str[1], "WB") == 0) || strcmp(y, "R") == 0)
                {
                    for (i = 0; i < arr[2] - 1; i++)
                    {
                        cache[s][i] = cache[s][i + 1];
                    }
                    cache[s][i] = t;
                }
                misscount++;
            }
        }
        h = 0;
        if (strcmp(str[0], "LRU") == 0)
        {
            for (int i = 0; i < arr[2]; i++)
            {
                if (cache[s][i] == t)
                {
                     printf("Address: 0x%s, Set: 0x%s ,Hit, Tag: 0x%s\n",hex,hex1,hex2);
                    hitcount++;
                    int d = cache[s][i];
                    int l = 0;
                    while (l != arr[2])
                    {
                        if (cache[s][l] == -1)
                            break;
                        l++;
                    }
                    int g = l - 1;
                    for (int j = i; j < g; j++)
                    {
                        cache[s][j] = cache[s][j + 1];
                    }
                    cache[s][g] = d;
                    h = 1;
                    break;
                }
                else if (cache[s][i] == -1)
                {
                     printf("Address: 0x%s, Set: 0x%s ,Miss,Tag: 0x%s\n",hex,hex1,hex2);
                    misscount++;
                    if ((strcmp(y, "W") == 0 && strcmp(str[1], "WB") == 0) || strcmp(y, "R") == 0)
                    {
                        cache[s][i] = t;
                    }
                    h = 1;
                    break;
                }
            }
            if (h == 0)
            {
                 printf("Address: 0x%s, Set: 0x%s ,Miss,Tag: 0x%s\n",hex,hex1,hex2);
                misscount++;
                int i;
                if ((strcmp(y, "W") == 0 && strcmp(str[1], "WB") == 0) || strcmp(y, "R") == 0)
                {
                    for (i = 0; i < arr[2] - 1; i++)
                    {
                        cache[s][i] = cache[s][i + 1];
                    }
                    cache[s][i] = t;
                }
            }
        }
        h = 0;
        if (strcmp(str[0], "RANDOM") == 0)
        {
            srand(time(NULL));
            for (int i = 0; i < arr[2]; i++)
            {
                if (cache[s][i] == t)
                {
                    printf("Address: 0x%s, Set: 0x%s ,Hit, Tag: 0x%s\n", hex, hex1, hex2);
                    h = 1;
                    hitcount++;
                    break;
                }
                else if (cache[s][i] == -1)
                {
                    printf("Address: 0x%s, Set: 0x%s ,Miss,Tag: 0x%s\n", hex, hex1, hex2);
                    if ((strcmp(y, "W") == 0 && strcmp(str[1], "WB") == 0) || strcmp(y, "R") == 0)
                    {
                        cache[s][i] = t;
                    }
                    h = 1;
                    misscount++;
                    break;
                }
            }
            if (h == 0)
            {
                printf("Address: 0x%s, Set: 0x%s ,Miss,Tag: 0x%s\n", hex, hex1, hex2);
                if ((strcmp(y, "W") == 0 && strcmp(str[1], "WB") == 0) || strcmp(y, "R") == 0)
                {
                    int randomindex = rand() % arr[2];
                    cache[s][randomindex] = t;
                }
                misscount++;
            }
        }
        c = 0;
    }
    printf("Total number of hits: %d\n",hitcount);
    printf("Total number of misses: %d\n",misscount);
}