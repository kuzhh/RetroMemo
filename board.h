#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "assets.h"

#define SCREEN_WIDTH 1366 //ancho
#define SCREEN_HEIGHT 768 //alto

#define SDL_MAIN_HANDLED

#define OK 0
#define ERR -1

#define LOW_ROWS 3
#define LOW_COLS 4

#define MID_ROWS 4
#define MID_COLS 4

#define HIGH_ROWS 4
#define HIGH_COLS 5

#define CARD_W 256
#define CARD_H 144

typedef struct
{
    SDL_Rect rect;
    SDL_Texture* cardFront;
    SDL_Texture* cardBack;
    int isFlipped;
    int isMatched;
    int id; //esto identifica los pares
} tCard; //esto ser�a una unica tarjeta

typedef struct
{
    tCard* cards; //esto no se si esta bien asi
    int rows; //depende del modo de juego
    int cols; //depende del modo de juego
    int totalCards; //depende del modo de juego
} tBoard;

void shuffleCards(tBoard*);
int boardInit(tBoard*, int, int);
void boardDestroy(tBoard*);
void boardRender(SDL_Renderer*, tBoard*, tCardSet*);

int boardGetCardAt(tBoard*, int, int);

#endif // BOARD_H_INCLUDED
