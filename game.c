#include "game.h"

#define ERR -1

/*
#include "input.h"
#include "assets.h"
#include "screen.h"      
#include <string.h>     
#include <stdlib.h>      
#include <time.h> // biblios que podriamos agregar, por las dudas lo dejo asi */


/*FUNCION PARA MEZCLAR CARTAS (capaz sirve) 
static void mezclar(int* a, int n)
{
    for(int i = 0; i < n - 1; i++)// recorro
    {
        int j = i + rand() % (n - i);// elijo una pos al azar

        int aux = a[i];// intercambio
        a[i] = a[j];
        a[j] = aux;
    }
}
*/

int gameInit(tGame* game, tBoard* board, int playerCount)
{
    int i;

    if(!game || !board)
        return ERR;

    game->state = STATE_PLAYING;
    game->isRunning = 1;

    game->playerCount = playerCount;
    game->currentPlayer = 0;

    game->startTime = 0;
    game->turnStartTime = 0;

    for(i = 0; i < playerCount; i++)
    {
        game->players[i].score = 0;
    }

    game->totalPairs = board->totalCards / 2;

    return OK;
}
