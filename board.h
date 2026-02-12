#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

typedef struct
{
    SDL_Rect rect;
    SDL_Texture* cardFront;
    SDL_Texture* cardBack;
    int isFlipped;
    int isMatched;
    int id; //esto identifica los pares
} tCard; //esto sería una unica tarjeta

typedef struct
{
    tCard* cards; //esto no se si esta bien asi
    int rows; //depende del modo de juego
    int cols; //depende del modo de juego
    int totalCards; //depende del modo de juego
} tBoard;

#endif // BOARD_H_INCLUDED
