#include "board.h"

void shuffleCards(tBoard* board)
{
    srand((unsigned int)time(NULL));

    for(int i = 0; i < board->totalCards; i++)
    {
        int j = rand() % board->totalCards;

        tCard temp = board->cards[i];
        board->cards[i] = board->cards[j];
        board->cards[j] = temp;
    }
}

int boardInit(tBoard* board, int rows, int cols)
{
    if(!board || rows <= 0 || cols <= 0)
        return ERR;

    board->rows = rows;
    board->cols = cols;
    board->totalCards = (rows * cols);

    if((board->totalCards == 0) || (board->totalCards % 2 != 0))
    {
        printf("Error board: we need pairs for playing.\n");
        return ERR;
    }

    board->cards = malloc(sizeof(tCard) * board->totalCards);
    if(!board->cards)
    {
        printf("Error creating board: no memory available");
        return ERR;
    }

    for(int i=0; i<board->totalCards; i++)
    {
        board->cards[i].isFlipped = 0;
        board->cards[i].isMatched = 0;
        board->cards[i].id = i / 2;
    }

    shuffleCards(board);

    return OK;
}

void boardDestroy(tBoard* board)
{
    if(board->cards)
    {
        free(board->cards);
        board->cards = NULL;
    }
}

void boardRender(SDL_Renderer* renderer, tBoard* board, tCardSet* card)
{
    if(!board->cards || !card)
        return;

    int hSpacing = 30;
    int wSpacing = 25;

    if(board->cols == 5)
        hSpacing = 15;

    int totalW = board->cols * CARD_W + (board->cols -1) * hSpacing;
    int totalH = board->rows * CARD_H + (board->rows -1) * wSpacing;

    int startX = (SCREEN_WIDTH -totalW) /2;
    int startY = (SCREEN_HEIGHT -totalH) /2;

    for(int i=0; i < board->totalCards; i++)
    {
        int row = i / board->cols;
        int col = i % board->cols;

        SDL_Rect dest = {
            .x = startX + col * (CARD_W + hSpacing),
            .y = startY + row * (CARD_H + wSpacing),
            .w = CARD_W,
            .h = CARD_H
        };

        if(board->cards[i].isFlipped || board->cards[i].isMatched)
        {
            SDL_RenderCopy(renderer, card->cardFront[board->cards[i].id], NULL, &dest);
        }
        else
        {
            SDL_RenderCopy(renderer, card->cardBack, NULL, &dest);
        }
    }
}

int boardGetCardAt(tBoard* board, int mouseX, int mouseY)
{
    if(!board->cards)
        return ERR;

    int hSpacing = 30;
    int wSpacing = 25;

    if(board->cols == 5)
        hSpacing = 15;

    int totalW = board->cols * CARD_W + (board->cols -1) * hSpacing;
    int totalH = board->rows * CARD_H + (board->rows -1) * wSpacing;

    int startX = (SCREEN_WIDTH -totalW) /2;
    int startY = (SCREEN_HEIGHT -totalH) /2;

    if(mouseX < startX || mouseY < startY)
        return -1;

    if(mouseX > startX + totalW || mouseY > startY + totalH)
        return -1;

    int col = (mouseX - startX) / (CARD_W + hSpacing);
    int row = (mouseY - startY) / (CARD_H + wSpacing);

    if(col < 0 || col >= board->cols || row < 0 || row >= board->rows)
        return -1;

    int cardX = startX + col * (CARD_W + hSpacing);
    int cardY = startY + row * (CARD_H + wSpacing);

    if(mouseX > cardX + CARD_W)
        return -1;

    if(mouseY > cardY + CARD_H)
        return -1;

    return row * board->cols +col;
}