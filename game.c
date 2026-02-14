#include "game.h"
//#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
debo armar
cant de jug
tamaño de tab
nombres ya cargados de p
reglas de puntaje

*/

//FUNCION PARA MEZCLAR CARTAS

static void mezclar(int* a, int n)//*a es donde estan las cartas y n la cant de cartas
{
    for(int i = 0; i < n - 1; i++)// recorro
    {
        int j = i + rand() % (n - i);// elijo una pos al azar

        int aux = a[i];// intercambio
        a[i] = a[j];
        a[j] = aux;
    }
}

void game_start(tGame* g, int rows, int cols)
{
    g->state = STATE_PLAYING;
    g->isRunning = 1;

    g->rows = rows;
    g->cols = cols;
    g->cardCount = rows * cols;

    // vlidación basica
    if (g->cardCount > MAX_CARDS || (g->cardCount % 2) != 0) {
        g->state = STATE_GAMEOVER;
        g->isRunning = 0;
        return;
    }

    g->totalPairs = g->cardCount / 2;

    // selección inicial
    g->selection.firstSelected = -1;
    g->selection.secondSelected = -1;
    g->selection.waiting = 0;
    g->selection.waitStart = 0;

    // creo ids por pares
    int ids[MAX_CARDS];
    int k = 0;
    for (int id = 0; id < g->totalPairs; id++) {
        ids[k++] = id;
        ids[k++] = id;
    }

    // mezclo solo los que se usan
    mezclar(ids, g->cardCount);

    // cargo cartas usadas
    for (int i = 0; i < g->cardCount; i++) {
        g->cards[i].id = ids[i];
        g->cards[i].faceUp = 0;
        g->cards[i].matched = 0;
    }

    /* (opcional)limpiar el resto para q este mas prolijo
    for (int i = g->cardCount; i < MAX_CARDS; i++) {
        g->cards[i].id = -1;
        g->cards[i].faceUp = 0;
        g->cards[i].matched = 0; */
    }

    g->startTime = 0;
    g->turnStartTime = 0;
}

void game_init(tGame* g)
{
    g->state = STATE_PLAYING;
    g->isRunning = 1;

    // defaults seguros
    g->playerCount = 0;
    g->currentPlayer = 0;

    for (int p = 0; p < MAX_PLAYERS; p++) {
        g->players[p].pairsFound = 0;
        g->players[p].moves = 0;
        g->players[p].score = 0;
        // NO toco namePlayer
    }

    g->rows = 0;
    g->cols = 0;
    g->cardCount = 0;
    g->totalPairs = 0;

    g->selection.firstSelected = -1;
    g->selection.secondSelected = -1;
    g->selection.waiting = 0;
    g->selection.waitStart = 0;

    for (int i = 0; i < MAX_CARDS; i++) {
        g->cards[i].id = -1;
        g->cards[i].faceUp = 0;
        g->cards[i].matched = 0;
    }

    g->startTime = 0;
    g->turnStartTime = 0;
}

