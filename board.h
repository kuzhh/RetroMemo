#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "library.h"

#include "assets.h"
#include "sound.h"
#include "vector.h"  // ADD: incluimos el TDA Vector para manejar las cartas con memoria dinámica (sin usar malloc directo acá)

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
    // MOD: antes era tCard* cards (malloc/free directo).
    // Ahora usamos tVector para cumplir la consigna: "solución principal con TDA Vector y memoria dinámica".
    // Importante: este vector guarda elementos de tipo tCard (elem_size = sizeof(tCard)).
    // NO debe accederse directamente desde otros módulos (usar boardCardAt).
    tVector cards;

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

// ADD: helper para obtener puntero a carta por índice.
// IMPORTANTE: reemplaza cualquier uso de &board->cards[i].
// Evita que el resto del juego dependa de la implementación interna (vector).
tCard* boardCardAt(tBoard* board, int index);

// ADD: helper para saber si el board está listo (vector inicializado).
// Reemplaza el viejo chequeo: if(board->cards != NULL)
int boardIsReady(const tBoard* board);

#endif // BOARD_H_INCLUDED
