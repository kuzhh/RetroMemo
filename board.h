#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "library.h"

#include "assets.h"
#include "sound.h"
#include "vector.h"  // incluimos el TDA Vector para manejar las cartas con mem din

typedef struct
{
    SDL_Rect rect;
    SDL_Texture* cardFront;
    SDL_Texture* cardBack;
    int isFlipped;
    int isMatched;
    int id; //esto identifica los pares
    tSound *sound_Click;//sonido seleccion
    tSound *sound_Matched;//sonido correcto
    tSound *sound_Not_Matched;//sonido fallo
} tCard; //esto seria una unica tarjeta

typedef struct
{
    // guarda elementos de tipo tCard (elem_size = sizeof(tCard)).
    tVector cards;

    int rows;
    int cols;
    int totalCards; 
} tBoard;

void shuffleCards(tBoard*);
int boardInit(tBoard*, int, int);
void boardDestroy(tBoard*);
void boardRender(SDL_Renderer*, tBoard*, tCardSet*);
void boardRenderHover(SDL_Renderer*, tBoard*, tCardSet*, int);

int boardGetCardAt(tBoard*, int, int);

tCard* boardCardAt(tBoard* board, int index);

int boardIsReady(const tBoard* board);

#endif // BOARD_H_INCLUDED

