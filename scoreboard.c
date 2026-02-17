#include "scoreboard.h"

static void sortDesc(tScoreEntry a[], int n)
{
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(a[j].score > a[i].score)
            {
                tScoreEntry tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
}

int scoreboardLoad(const char* filename, tScoreEntry out[], int* outCount)
{
    FILE* f = fopen(filename, "r");
    if(!outCount) 
        return 0;
        
    *outCount = 0;

    if(!f)  
        return 1; // si no existe, arrancamos vacío (no es error)

    while(*outCount < SCORE_MAX_ENTRIES)
    {
        char name[SCORE_NAME_MAX];
        int score;

        if(fscanf(f, " %63[^,],%d", name, &score) != 2)
            break;

        strncpy(out[*outCount].name, name, SCORE_NAME_MAX-1);
        out[*outCount].name[SCORE_NAME_MAX-1] = '\0';
        out[*outCount].score = score;
        (*outCount)++;

        int c;
        while((c = fgetc(f)) != '\n' && c != EOF) {}
    }

    fclose(f);
    sortDesc(out, *outCount);
    return 1;
}

int scoreboardSave(const char* filename, const tScoreEntry entries[], int count)
{
    FILE* f = fopen(filename, "w");
    if(!f) return 0;

    for(int i=0; i<count; i++)
        fprintf(f, "%s,%d\n", entries[i].name, entries[i].score);

    fclose(f);
    return 1;
}

void scoreboardInsertTop5(tScoreEntry entries[], int* count, const char* name, int score)
{
    if(!count || !name) return;

    if(*count < SCORE_MAX_ENTRIES)
    {
        strncpy(entries[*count].name, name, SCORE_NAME_MAX-1);
        entries[*count].name[SCORE_NAME_MAX-1] = '\0';
        entries[*count].score = score;
        (*count)++;
        sortDesc(entries, *count);
        return;
    }

    sortDesc(entries, *count);
    int minIdx = *count - 1;

    if(score <= entries[minIdx].score)
        return; // no entra al top 5

    strncpy(entries[minIdx].name, name, SCORE_NAME_MAX-1);
    entries[minIdx].name[SCORE_NAME_MAX-1] = '\0';
    entries[minIdx].score = score;
    sortDesc(entries, *count);
}
