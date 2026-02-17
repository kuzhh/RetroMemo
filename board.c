#include "board.h"

static SDL_Rect rectScaleCentered(SDL_Rect r, float scale)
{
    int newW = (int)(r.w * scale);
    int newH = (int)(r.h * scale);
    r.x -= (newW - r.w) / 2;
    r.y -= (newH - r.h) / 2;
    r.w = newW;
    r.h = newH;
    return r;
}

void shuffleCards(tBoard *board)
{
    srand((unsigned int)time(NULL));

    for (int i = 0; i < board->totalCards; i++)
    {
        int j = rand() % board->totalCards;

        // MOD: antes era swap directo con board->cards[i] (porque era tCard*).
        // Ahora las cartas están dentro del tVector, entonces accedemos con vector_get.
        tCard *ci = (tCard *)vector_get(&board->cards, i);
        tCard *cj = (tCard *)vector_get(&board->cards, j);

        // ADD: blindaje por si index inválido (no debería pasar, pero evita crasheos)
        if (!ci || !cj)
            continue;

        // MOD: swap de contenido de las cartas (intercambio structs)
        tCard temp = *ci;
        *ci = *cj;
        *cj = temp;
    }
}

int boardInit(tBoard *board, int rows, int cols)
{
    if (!board || rows <= 0 || cols <= 0)
        return ERR;

    board->rows = rows;
    board->cols = cols;
    board->totalCards = (rows * cols);

    if ((board->totalCards == 0) || (board->totalCards % 2 != 0))
    {
        printf("Error board: we need pairs for playing.\n");
        return ERR;
    }

    // MOD: antes se hacía malloc directo de tCard*.
    // Ahora inicializamos el TDA Vector (memoria dinámica encapsulada), cumpliendo la consigna.
    if (!vector_init(&board->cards, sizeof(tCard))) // ADD: crea vector para guardar tCard
    {
        printf("Error creating board: no memory available");
        return ERR;
    }

    for (int i = 0; i < board->totalCards; i++)
    {
        // ADD: armamos una carta "local" y la pusheamos al vector
        tCard newCard;

        // MOD: antes era board->cards[i].campo = ...
        // Ahora llenamos newCard y lo agregamos al vector.
        newCard.isFlipped = 0;
        newCard.isMatched = 0;
        newCard.id = i / 2;

        // ADD: estos campos no los estabas usando acá (rect/texturas), pero los inicializo
        // para evitar basura en memoria. Si los seteás en otro lado, igual está ok.
        newCard.rect = (SDL_Rect){0, 0, 0, 0};
        newCard.cardFront = NULL;
        newCard.cardBack = NULL;

        // cargo sonido cartas
        // cargo sonido cartas
        newCard.sound_Click = sound_load(SOUND_CLICK);
        newCard.sound_Matched = sound_load(SOUND_MATCHED);
        newCard.sound_Not_Matched = sound_load(SOUND_NOT_MATCHED);

        // ADD: agrego la carta al vector
        if (!vector_push_back(&board->cards, &newCard))
        {
            // ADD: si falla el push, libero lo que ya haya y aviso error
            printf("Error creating board: could not push card");
            boardDestroy(board); // usa destroy vector + sonidos ya cargados
            return ERR;
        }
    }

    shuffleCards(board);

    return OK;
}

void boardDestroy(tBoard *board)
{
    // MOD: antes chequeabas board->cards (puntero). Ahora es un vector.
    // Si el vector no tiene data, no hay nada para destruir.
    if (!board)
        return;

    // ADD: recorremos las cartas dentro del vector para destruir sonidos
    for (int i = 0; i < board->totalCards; i++)
    {
        tCard *c = (tCard *)vector_get(&board->cards, i); // ADD: obtengo carta del vector
        if (!c)
            continue;

        sound_destroy(c->sound_Matched);
        sound_destroy(c->sound_Not_Matched);

        // ADD: (opcional) dejo punteros en NULL por prolijidad
        c->sound_Matched = NULL;
        c->sound_Not_Matched = NULL;
    }

    // MOD: antes era free(board->cards). Ahora destruimos el vector.
    vector_destroy(&board->cards); // ADD: libera memoria dinámica del vector
}

void boardRender(SDL_Renderer *renderer, tBoard *board, tCardSet *card)
{
    // MOD: antes era !board->cards. Ahora chequeo que el vector tenga data.
    // Si tu vector usa data=NULL cuando está vacío, esto está perfecto.
    if (!board || !board->cards.data || !card)
        return;

    int hSpacing = 30;
    int wSpacing = 25;
    int cardW = CARD_W;
    int cardH = CARD_H;

    if (board->cols == 5)
    {
        hSpacing = 15;
        cardW = 200;
        cardH = 112;
    }

    int totalW = board->cols * cardW + (board->cols - 1) * hSpacing;
    int totalH = board->rows * cardH + (board->rows - 1) * wSpacing;

    int startX = (SCREEN_WIDTH - totalW) / 2;
    int startY = (SCREEN_HEIGHT - totalH) / 2;

    for (int i = 0; i < board->totalCards; i++)
    {
        int row = i / board->cols;
        int col = i % board->cols;

        SDL_Rect dest = {
            .x = startX + col * (cardW + hSpacing),
            .y = startY + row * (cardH + wSpacing),
            .w = cardW,
            .h = cardH
        };

        // ADD: obtengo carta desde el vector
        tCard *c = (tCard *)vector_get(&board->cards, i);
        if (!c)
            continue;

        // MOD: antes era board->cards[i].isFlipped etc.
        if (c->isFlipped || c->isMatched)
        {
            SDL_RenderCopy(renderer, card->cardFront[c->id], NULL, &dest);
        }
        else
        {
            SDL_RenderCopy(renderer, card->cardBack, NULL, &dest);
        }
    }
}

int boardGetCardAt(tBoard *board, int mouseX, int mouseY)
{
    // MOD: antes chequeabas board->cards (puntero). Ahora chequeo data del vector.
    if (!board || !board->cards.data) // evita errores
        return -1;                    // evita errores

    int hSpacing = 30;
    int wSpacing = 25;
    int cardW = CARD_W;
    int cardH = CARD_H;

    if (board->cols == 5)
    {
        hSpacing = 15;
        cardW = 200;
        cardH = 112;
    }

    int totalW = board->cols * cardW + (board->cols - 1) * hSpacing;
    int totalH = board->rows * cardH + (board->rows - 1) * wSpacing;

    int startX = (SCREEN_WIDTH - totalW) / 2;
    int startY = (SCREEN_HEIGHT - totalH) / 2;

    if (mouseX < startX || mouseY < startY)
        return -1;

    if (mouseX > startX + totalW || mouseY > startY + totalH)
        return -1;

    int col = (mouseX - startX) / (cardW + hSpacing);
    int row = (mouseY - startY) / (cardH + wSpacing);

    if (col < 0 || col >= board->cols || row < 0 || row >= board->rows)
        return -1;

    int cardX = startX + col * (cardW + hSpacing);
    int cardY = startY + row * (cardH + wSpacing);

    if (mouseX >= cardX + cardW)
        return -1;
    if (mouseY >= cardY + cardH)
        return -1;

    return row * board->cols + col;
}

void boardRenderHover(SDL_Renderer *renderer, tBoard *board, tCardSet *card, int hovered)
{
    // MOD: antes era !board->cards. Ahora chequeo data del vector.
    if (!board || !board->cards.data || !card)
        return;

    int hSpacing = 30;
    int wSpacing = 25;
    int cardW = CARD_W;
    int cardH = CARD_H;

    if (board->cols == 5)
    {
        hSpacing = 15;
        cardW = 200;
        cardH = 112;
    }

    int totalW = board->cols * cardW + (board->cols - 1) * hSpacing;
    int totalH = board->rows * cardH + (board->rows - 1) * wSpacing;

    int startX = (SCREEN_WIDTH - totalW) / 2;
    int startY = (SCREEN_HEIGHT - totalH) / 2;

    for (int i = 0; i < board->totalCards; i++)
    {
        int row = i / board->cols;
        int col = i % board->cols;

        SDL_Rect dest = {
            .x = startX + col * (cardW + hSpacing),
            .y = startY + row * (cardH + wSpacing),
            .w = cardW,
            .h = cardH
        };

        // ADD: obtengo carta desde el vector
        tCard *c = (tCard *)vector_get(&board->cards, i);
        if (!c)
            continue;

        // MOD: antes era board->cards[i].isMatched etc.
        if (i == hovered && !c->isMatched && !c->isFlipped)
        {
            dest = rectScaleCentered(dest, 1.10f);
            dest.y -= 4;
        }

        if (c->isFlipped || c->isMatched)
        {
            SDL_RenderCopy(renderer, card->cardFront[c->id], NULL, &dest);
        }
        else
        {
            SDL_RenderCopy(renderer, card->cardBack, NULL, &dest);
        }
    }
}
tCard* boardCardAt(tBoard* board, int index)
{
    // ADD: blindaje, evita crasheos si index está fuera de rango
    if (!board || !board->cards.data || index < 0 || index >= board->totalCards)
        return NULL;

    return (tCard*)vector_get(&board->cards, (size_t)index);
}

int boardIsReady(const tBoard* board)
{
    // ADD: esto reemplaza el viejo "board->cards != NULL"
    return (board && board->cards.data != NULL);
}
