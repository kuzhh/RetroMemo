#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

#define SCORE_MAX_ENTRIES 5
#define SCORE_NAME_MAX    64

typedef struct
{
    char name[SCORE_NAME_MAX];
    int score;
} tScoreEntry;

int scoreboardLoad(const char* filename, tScoreEntry out[], int* outCount);
int scoreboardSave(const char* filename, const tScoreEntry entries[], int count);
void scoreboardInsertTop5(tScoreEntry entries[], int* count, const char* name, int score);

#endif
