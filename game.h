#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
/*
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h> */

#include <stdint.h> //esto lo agregue yo
#include "input.h"

#define MAX_PLAYERS 2


//agregado
#define BOARD_ROWS 4
#define BOARD_COLS 4
#define MAX_CARDS 20//que el max. de cards sea por tablero

typedef struct {
    int id;        // 0..totalpares-1
    int faceUp;    // 0/1
    int matched;   // 0/1
} tCard;
//agregado


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

typedef struct //lo dejo al final para ver que usar
{
    GameState state;
    int isRunning;

    tPlayer players[MAX_PLAYERS];
    int playerCount;
    int currentPlayer;

    int rows, cols;
    int cardCount;     // rows*cols
    int totalPairs;    // cardCount/2

    Uint32 startTime;
    Uint32 turnStartTime;

    tCard cards[MAX_CARDS];
    tSelection selection;

} tGame;

void game_init(tGame* g);// una vez
void game_start(tGame* g, int r, int c);// cada partida


#endif // GAME_H_INCLUDED
