#include "game.h"
#include "scoreboard.h"
#include "board.h"

/*  Inicializa el juego:
    -setea estado inicial
    -setea cantidad de jugadores y turno inicial
    -inicializa contadores y puntajes (IMPORTANTE: también pairsFound y moves)
    -calcula totalPairs a partir del board
*/

int gameInit(tGame *game, tBoard *board, int playerCount)
{
    int i;

    if (!game || !board)
        return ERR;

    if (playerCount > MAX_PLAYERS)
        return ERR;

    game->state = STATE_PLAYING;
    game->isRunning = 1;

    game->playerCount = playerCount;
    game->currentPlayer = 0;

    game->startTime = 0;
    game->turnStartTime = 0;

    for (i = 0; i < playerCount; i++)
    {
        game->players[i].score = 0;
        game->players[i].pairsFound = 0; // evito basura ADD
        game->players[i].moves = 0;      // idem ADD
        game->players[i].streak = 0;     // idem
    }

    game->totalPairs = board->totalCards / 2;

    return OK;
}

/*  Se llama cuando el board ya resolvió un intento.
    informa si hubo match o no.

    Reglas actuales (modo simple):
    - Gana el que MÁS PARES encontró.
    - Match:
        * incrementa pairsFound del jugador actual
        * score = pairsFound (o sea, score representa cantidad de pares)
        * el jugador sigue (no cambia currentPlayer)
    - No match:
        * no suma pares / no cambia score
        * si hay 2 jugadores, cambia turno (0 <-> 1)
*/
void gameOnPairResolved(tGame *game, int isMatch)
{
    if (!game || game->playerCount <= 0)
        return;

    // MOVIMIENTOS -> Cuenta intentos (cada comparación de 2 cartas).
    game->players[game->currentPlayer].moves++;

    if (isMatch)
    {
        // Incrementa pares encontrados
        game->players[game->currentPlayer].pairsFound++;

        // Incrementa racha
        game->players[game->currentPlayer].streak++;

        // SCORE: Ahora cada par vale 100 puntos.
        int basePoints = 100;

        // Si tiene racha, incremento
        if (game->players[game->currentPlayer].streak >= 2)
        {
            basePoints = (int)(basePoints * 1.75);
        }

        /*
        Antes estaba:
        game->players[game->currentPlayer].score =
            game->players[game->currentPlayer].pairsFound * 100;
        Al tener el score inicializado en 0 (ver gameInit()) y
        los puntos base iniciados en basePoint (ver arriba)
        se reemplaza por lo siguiente:
        */
        game->players[game->currentPlayer].score += basePoints;
        /*pd: si es racha se suma con incremento; si no es racha
        suma sólo los pts base*/

        // El jugador continúa su turno
    }
    else
    {
        // Si falla, no tiene más racha
        game->players[game->currentPlayer].streak = 0;

        // y se le aplica penalización
        game->players[game->currentPlayer].score -= 10;
        // pero si score < 0, lo deja en 0
        if (game->players[game->currentPlayer].score < 0)
            game->players[game->currentPlayer].score = 0;

        // cambia turno (solo en MP)
        if (game->playerCount == 2)
            game->currentPlayer = 1 - game->currentPlayer;
    }
}

/*  Determina el ganador al final (modo 2 jugadores)
    -devuelve 0 si gana player 0
    -devuelve 1 si gana player 1
    -devuelve -1 si hay empate
    -si hay 1 jugador, devuelve 0 (es el único)
*/
int gameGetWinnerIndex(const tGame *game)
{
    if (!game || game->playerCount <= 0)
        return -1;

    if (game->playerCount == 1)
        return 0;

    // 2 jugadores
    int p0 = game->players[0].pairsFound;
    int p1 = game->players[1].pairsFound;

    if (p0 > p1)
        return 0;
    if (p1 > p0)
        return 1;
    return -1; // empate
}

/*  Guarda los puntajes actuales en el archivo SCORE_FILE (ranking Top 5):
    -carga lo que ya existe en el archivo
    -inserta los jugadores actuales (1 o 2) <<<manteniendo Top 5>>>>
    -guarda el ranking actualizado
*/
int gameCommitScoresToFile(const tGame *game) // guardar score
{
    if (!game)
        return 0;

    tScoreEntry entries[SCORE_MAX_ENTRIES];
    int count = 0;

    if (!scoreboardLoad(SCORE_FILE, entries, &count))
        return 0;

    for (int i = 0; i < game->playerCount; i++)
    {
        scoreboardInsertTop5(entries, &count, game->players[i].namePlayer,
                             game->players[i].score);
    }

    if (!scoreboardSave(SCORE_FILE, entries, count))
        return 0;

    return 1;
}