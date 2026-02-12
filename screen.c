#include "screen.h"

int lblCreate(tLabel* lbl, SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if(!surface)
    {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return SDL_ERR;
    }

    lbl->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if(!lbl->texture)
    {
        printf("Error creating text texture: %s\n", SDL_GetError());
        return SDL_ERR;
    }

    SDL_QueryTexture(lbl->texture, NULL, NULL, &lbl->rect.w, &lbl->rect.h);

    return OK;
}

int screenInitialize(tScreen* screen, const char* title, int w, int h)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)) //corregir esto pq no necesitas todos los recursos
    {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return SDL_ERR;
    }

    int img_init = IMG_Init(IMG_FLAGS);
    if((img_init & IMG_FLAGS) != IMG_FLAGS)
    {
        printf("Error inicializando IMG INIT PNG: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    if(TTF_Init())
    {
        printf("Error inicializando TTF INIT: %s\n", IMG_GetError());
        return SDL_ERR;
    }

    screen->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    if(!screen->window)
    {
        printf("Error inicializando SCREEN WINDOW: %s\n", SDL_GetError());
        return SDL_ERR;
    }

    screen->renderer = SDL_CreateRenderer(screen->window, -1, 0);
    if(!screen->renderer)
    {
        printf("Error inicializando SCREEN RENDERER: %s\n", SDL_GetError());
        return SDL_ERR;
    }

    return OK;
}

void screenShutdown(tScreen* screen)
{
    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int mainMenuInit(tMainMenu* menu, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int btnWidth = 240;
    int btnHeight = 60;
    int logoWidth = 158;
    int logoHeight = 210;

    //logo
    //menu->logoRect = (SDL_Rect){250, 60, 300, 120};
    menu->logoRect.w = logoWidth;
    menu->logoRect.h = logoHeight;
    menu->logoRect.x = centerX - (logoWidth / 2);
    menu->logoRect.y = 50;

    //btn SinglePlayer
    //menu->btnSP.rect = (SDL_Rect){300,250,200,50};
    menu->btnSP.rect.w = btnWidth;
    menu->btnSP.rect.h = btnHeight;
    menu->btnSP.rect.x = centerX - (btnWidth / 2);
    menu->btnSP.rect.y = centerY;

    menu->btnSP.state = BTN_NORMAL;

    //btn MultiPlayer
    menu->btnMP = menu->btnSP;
    menu->btnMP.rect.y += 60;

    menu->btnExit = menu->btnSP;
    menu->btnExit.rect.y += 120;

    SDL_Color white = {255,255,255,255};

    if(lblCreate(&menu->lblSP, renderer, assets->font, "Singleplayer", white) != OK)
        return SDL_ERR;
    
    menu->lblSP.rect.x = menu->btnSP.rect.x + (menu->btnSP.rect.w - menu->lblSP.rect.w)/2;
    menu->lblSP.rect.y = menu->btnSP.rect.y + (menu->btnSP.rect.h - menu->lblSP.rect.h)/2;

    if(lblCreate(&menu->lblMP, renderer, assets->font, "Multiplayer", white) != OK)
        return SDL_ERR;

    menu->lblMP.rect.x = menu->btnMP.rect.x + (menu->btnMP.rect.w - menu->lblMP.rect.w)/2;
    menu->lblMP.rect.y = menu->btnMP.rect.y + (menu->btnMP.rect.h - menu->lblMP.rect.h)/2;

    if(lblCreate(&menu->lblExit, renderer, assets->font, "Exit game", white) != OK)
        return SDL_ERR;

    menu->lblExit.rect.x = menu->btnExit.rect.x + (menu->btnExit.rect.w - menu->lblExit.rect.w)/2;
    menu->lblExit.rect.y = menu->btnExit.rect.y + (menu->btnExit.rect.h - menu->lblExit.rect.h)/2;

    return OK;
}

void mainMenuUpdate(tMainMenu* menu, tInput* input, ScreenType* currentScreen)
{
    btnUpdate(&menu->btnSP, input);
    btnUpdate(&menu->btnMP, input);
    btnUpdate(&menu->btnExit, input);

    if(menu->btnSP.state == BTN_PRESSED && input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &menu->btnSP.rect))
        *currentScreen = SCREEN_GAME;

    if(menu->btnExit.state == BTN_HOVER && input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &menu->btnExit.rect))
        *currentScreen = SCREEN_EXIT;
}

void mainMenuRender(SDL_Renderer* renderer, tMainMenu* menu, tAssets* assets)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);

    SDL_RenderCopy(renderer, assets->logo, NULL, &menu->logoRect);

    SDL_Texture* tex;

    switch(menu->btnSP.state)
    {
    case BTN_NORMAL:
        tex = assets->buttonNormal;
        break;

    case BTN_HOVER:
        tex = assets->buttonHover;
        break;

    case BTN_PRESSED:
        tex = assets->buttonPressed;
        break;

    default:
        tex = assets->buttonNormal;
        break;
    }

    SDL_RenderCopy(renderer, tex, NULL, &menu->btnSP.rect);

    switch(menu->btnMP.state)
    {
    case BTN_NORMAL:
        tex = assets->buttonNormal;
        break;

    case BTN_HOVER:
        tex = assets->buttonHover;
        break;

    case BTN_PRESSED:
        tex = assets->buttonPressed;
        break;

    default:
        tex = assets->buttonNormal;
        break;
    }

    SDL_RenderCopy(renderer, tex, NULL, &menu->btnMP.rect);


    switch(menu->btnExit.state)
    {
    case BTN_NORMAL:
        tex = assets->buttonNormal;
        break;

    case BTN_HOVER:
        tex = assets->buttonHover;
        break;

    case BTN_PRESSED:
        tex = assets->buttonPressed;
        break;

    default:
        tex = assets->buttonNormal;
        break;
    }

    SDL_RenderCopy(renderer, tex, NULL, &menu->btnExit.rect);

    SDL_RenderCopy(renderer, menu->lblSP.texture, NULL, &menu->lblSP.rect);
    SDL_RenderCopy(renderer, menu->lblMP.texture, NULL, &menu->lblMP.rect);
    SDL_RenderCopy(renderer, menu->lblExit.texture, NULL, &menu->lblExit.rect);
    
}

void mainMenuDestroy(tMainMenu* menu)
{
    if(menu->lblSP.texture)
        SDL_DestroyTexture(menu->lblSP.texture);

    if(menu->lblMP.texture)
        SDL_DestroyTexture(menu->lblMP.texture);

    if(menu->lblExit.texture)
        SDL_DestroyTexture(menu->lblExit.texture);
}