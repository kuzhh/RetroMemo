#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "library.h"

#include "input.h"
#include "board.h"

typedef enum {STATE_PLAYING, STATE_GAMEOVER} GameState;

typedef struct
{
    int id;
    int pairsFound;
    int moves;
    int score;
    char namePlayer[MAX_TEXT_INPUT];
    int streak;
} tPlayer;

typedef struct
{
    GameState state;
    int isRunning;

    tPlayer players[MAX_PLAYERS];
    int playerCount;
    int currentPlayer;

    int totalPairs;
    Uint32 startTime;
    Uint32 turnStartTime;
} tGame;

typedef struct
{
    int firstSelected;
    int secondSelected;
    int waiting;
    Uint32 waitStart;
} tSelection;

typedef struct
{
    int pointsPerPair;
    int penalty;
} tScoreRules;

int gameInit(tGame*, tBoard*, int);
void gameOnPairResolved(tGame* game, int isMatch);
int gameCommitScoresToFile(const tGame* game);
int gameGetWinnerIndex(const tGame* game);

#endif // GAME_H_INCLUDED
