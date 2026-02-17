#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "library.h"

#include "assets.h"
#include "sound.h"


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
    tCard* cards; //esto no se si esta bien asi
    int rows; //depende del modo de juego
    int cols; //depende del modo de juego
    int totalCards; //depende del modo de juego
} tBoard;

void shuffleCards(tBoard*);
int boardInit(tBoard*, int, int);
void boardDestroy(tBoard*);
void boardRender(SDL_Renderer*, tBoard*, tCardSet*);
void boardRenderHover(SDL_Renderer*, tBoard*, tCardSet*, int);

int boardGetCardAt(tBoard*, int, int);

#endif // BOARD_H_INCLUDED
