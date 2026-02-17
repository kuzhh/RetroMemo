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

        tCard temp = board->cards[i];
        board->cards[i] = board->cards[j];
        board->cards[j] = temp;
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

    board->cards = malloc(sizeof(tCard) * board->totalCards);
    if (!board->cards)
    {
        printf("Error creating board: no memory available");
        return ERR;
    }

    for (int i = 0; i < board->totalCards; i++)
    {
        board->cards[i].isFlipped = 0;
        board->cards[i].isMatched = 0;
        board->cards[i].id = i / 2;

        // cargo sonido cartas
        board->cards[i].sound_Click = sound_load(SOUND_CLICK);
        board->cards[i].sound_Matched = sound_load(SOUND_MATCHED);
        board->cards[i].sound_Not_Matched = sound_load(SOUND_NOT_MATCHED);
    }

    shuffleCards(board);

    return OK;
}

void boardDestroy(tBoard *board)
{
    if (!board->cards)
        return;

    for (int i = 0; i < board->totalCards; i++)
    {
        sound_destroy(board->cards[i].sound_Matched);
        sound_destroy(board->cards[i].sound_Not_Matched);
    }

    free(board->cards);
    board->cards = NULL;
}

void boardRender(SDL_Renderer *renderer, tBoard *board, tCardSet *card)
{
    if (!board->cards || !card)
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

        if (board->cards[i].isFlipped || board->cards[i].isMatched)
        {
            SDL_RenderCopy(renderer, card->cardFront[board->cards[i].id], NULL, &dest);
        }
        else
        {
            SDL_RenderCopy(renderer, card->cardBack, NULL, &dest);
        }
    }
}

int boardGetCardAt(tBoard *board, int mouseX, int mouseY)
{
    if (!board || !board->cards) // evita errores
        return -1;               // evita errores

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
    if (!board || !board->cards || !card)
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

        if (i == hovered && !board->cards[i].isMatched && !board->cards[i].isFlipped)
        {
            dest = rectScaleCentered(dest, 1.10f);
            dest.y -= 4;
        }

        if (board->cards[i].isFlipped || board->cards[i].isMatched)
        {
            SDL_RenderCopy(renderer, card->cardFront[board->cards[i].id], NULL, &dest);
        }
        else
        {
            SDL_RenderCopy(renderer, card->cardBack, NULL, &dest);
        }
    }
}
