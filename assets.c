#include "assets.h"

int cardsetLoad(tCardSet* cardSet, SDL_Renderer* renderer, CardSetType type)
{
    const char** card_paths = (type == CSET_DS) ? ds_card_paths : greek_card_paths;
    const char* card_back = (type == CSET_DS) ? SET_CARD_BACK_1 : SET_CARD_BACK_2;

    for(int i = 0; i < MAX_CARD_TYPES; i++)
    {
        cardSet->cardFront[i] = IMG_LoadTexture(renderer, card_paths[i]);
        if(!cardSet->cardFront[i])
        {
            printf("Error loading %s: %s\n", card_paths[i], IMG_GetError());
            cardsetUnload(cardSet);
            return SDL_ERR;
        }
    }

    cardSet->cardBack = IMG_LoadTexture(renderer, card_back);
    if(!cardSet->cardBack)
    {
        printf("Error loading card back: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    return OK;
}

void cardsetUnload(tCardSet* cardSet)
{
    for(int i= 0; i<MAX_CARD_TYPES; i++)
    {
        if(cardSet->cardFront[i])
        {
            SDL_DestroyTexture(cardSet->cardFront[i]);
            cardSet->cardFront[i] = NULL;
        }
    }

    if(cardSet->cardBack)
    {
        SDL_DestroyTexture(cardSet->cardBack);
        cardSet->cardBack = NULL;
    }
}

int assetsLoad(tAssets* assets, SDL_Renderer* renderer)
{
    memset(assets, 0, sizeof(tAssets));

    assets->background = IMG_LoadTexture(renderer, BACKGROUND_FIRST);
    if(!assets->background)
    {
        printf("Error loading background: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->backgroundGame = IMG_LoadTexture(renderer, BACKGROUND_SECOND);
    if(!assets->backgroundGame)
    {
        printf("Error loading backgroundGame: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->logo = IMG_LoadTexture(renderer, LOGO);
    if(!assets->logo)
    {
        printf("Error loading logo: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->scoreBox = IMG_LoadTexture(renderer, SCORE_BOX);
    if(!assets->scoreBox)
    {
        printf("Error loading scoreBox: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->buttonNormal = IMG_LoadTexture(renderer, BUTTON_NORMAL);
    if(!assets->buttonNormal)
    {
        printf("Error loading button normal: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->buttonHover = IMG_LoadTexture(renderer, BUTTON_HOVER);
    if(!assets->buttonHover)
    {
        printf("Error loading button hover: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->buttonPressed = IMG_LoadTexture(renderer, BUTTON_PRESSED);
    if(!assets->buttonPressed)
    {
        printf("Error loading button pressed: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->back = IMG_LoadTexture(renderer, BUTTON_BACK);
    if(!assets->back)
    {
        printf("Error loading back: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->font = TTF_OpenFont(FONT_PATCH, FONT_SIZE + 4);
    if(!assets->font)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        return SDL_ERR;
    }

    if(cardsetLoad(&assets->dsSet, renderer, CSET_DS) != OK)
        return SDL_ERR;

    if(cardsetLoad(&assets->greekSet, renderer, CSET_GREEK) != OK)
        return SDL_ERR;

    return OK;
}

void assetsUnload(tAssets* assets)
{
    if(assets->background)
        SDL_DestroyTexture(assets->background);

    if(assets->backgroundGame)
        SDL_DestroyTexture(assets->backgroundGame);

    if(assets->scoreBox)
        SDL_DestroyTexture(assets->scoreBox);

    if(assets->logo)
        SDL_DestroyTexture(assets->logo);

    if(assets->buttonNormal)
        SDL_DestroyTexture(assets->buttonNormal);
    if(assets->buttonHover)
        SDL_DestroyTexture(assets->buttonHover);
    if(assets->buttonPressed)
        SDL_DestroyTexture(assets->buttonPressed);
    if(assets->back)
        SDL_DestroyTexture(assets->back);

    if(assets->font)
        TTF_CloseFont(assets->font);

    cardsetUnload(&assets->dsSet);
    cardsetUnload(&assets->greekSet);

    memset(assets, 0, sizeof(tAssets));
}

void btnUpdate(tButton* btn, tInput* input)
{
    int inside = pointInRect(input->mouseX, input->mouseY, &btn->rect);

    switch(btn->state)
    {
    case BTN_NORMAL:
        if(inside)
            btn->state = BTN_HOVER;
        break;

    case BTN_HOVER:
        if(!inside)
            btn->state = BTN_NORMAL;
        else if(input->mousePressed)
            btn->state = BTN_PRESSED;
        break;

    case BTN_PRESSED:
        if(!input->mouseDown)
        {
            if(inside)
                btn->state = BTN_HOVER;
            else
                btn->state = BTN_NORMAL;
        }
        break;
    }
}
