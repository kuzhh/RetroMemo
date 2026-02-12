#include "assets.h"

int assetsLoad(tAssets* assets, SDL_Renderer* renderer)
{
    memset(assets, 0, sizeof(tAssets));

    assets->background = IMG_LoadTexture(renderer, "img/background1.png");
    if(!assets->background)
    {
        printf("Error loading bg: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->logo = IMG_LoadTexture(renderer, "img/logo.png");
    if(!assets->logo)
    {
        printf("Error loading logo: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    assets->buttonNormal = IMG_LoadTexture(renderer, "img/btn_normal.png");
    if(!assets->buttonNormal)
        return SDL_ERR;

    assets->buttonHover = IMG_LoadTexture(renderer, "img/btn_hover.png");
    if(!assets->buttonHover)
        return SDL_ERR;

    assets->buttonPressed = IMG_LoadTexture(renderer, "img/btn_normal.png");
    if(!assets->buttonPressed)
        return SDL_ERR;

    assets->font = TTF_OpenFont("fnt/PixelifySans-VariableFont_wght.ttf", 28);
    if(!assets->font)
    {
        printf("Error loading FONT: %s\n", TTF_GetError());
        return SDL_ERR;
    }

    return OK;
}

void assetsUnload(tAssets* assets)
{
    if(assets->background)
        SDL_DestroyTexture(assets->background);

    if(assets->logo)
        SDL_DestroyTexture(assets->logo);

    if(assets->buttonNormal)
        SDL_DestroyTexture(assets->buttonNormal);
    if(assets->buttonHover)
        SDL_DestroyTexture(assets->buttonHover);
    if(assets->buttonPressed)
        SDL_DestroyTexture(assets->buttonPressed);

    if(assets->font)
        TTF_CloseFont(assets->font);

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
