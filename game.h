#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "input.h"
#include "board.h"

#define MAX_PLAYERS 2

typedef enum {STATE_PLAYING, STATE_GAMEOVER} GameState;

typedef struct
{
    int id;
    int pairsFound;
    int moves;
    int score;
    char namePlayer[MAX_TEXT_INPUT];
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

#endif // GAME_H_INCLUDED
