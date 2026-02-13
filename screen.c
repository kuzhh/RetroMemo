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
    int btnWidth = BTN_W;
    int btnHeight = BTN_H;
    int logoWidth = LOGO_W;
    int logoHeight = LOGO_H;

    //logo
    //menu->logoRect = (SDL_Rect){250, 60, 300, 120};
    menu->logoRect.w = logoWidth;
    menu->logoRect.h = logoHeight;
    menu->logoRect.x = centerX - (logoWidth / 2);
    menu->logoRect.y = 10;

    //btn SinglePlayer
    //menu->btnSP.rect = (SDL_Rect){300,250,200,50};
    menu->btnSP.rect.w = btnWidth;
    menu->btnSP.rect.h = btnHeight;
    menu->btnSP.rect.x = centerX - (btnWidth / 2);
    menu->btnSP.rect.y = centerY + 50;

    menu->btnSP.state = BTN_NORMAL;

    //btn MultiPlayer
    menu->btnMP = menu->btnSP;
    menu->btnMP.rect.y += 90;

    menu->btnExit = menu->btnSP;
    menu->btnExit.rect.y += 180;

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

    //DEBUG - Solo mostrar si hay click
    if(input->mouseReleased)
    {
        input->mouseX, input->mouseY, menu->btnSP.state, menu->btnMP.state, menu->btnExit.state;
        menu->btnSP.rect.x, menu->btnSP.rect.y, menu->btnSP.rect.w, menu->btnSP.rect.h;
        pointInRect(input->mouseX, input->mouseY, &menu->btnSP.rect),
        pointInRect(input->mouseX, input->mouseY, &menu->btnMP.rect),
        pointInRect(input->mouseX, input->mouseY, &menu->btnExit.rect);
    }

    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &menu->btnSP.rect))
    {
        *currentScreen = SCREEN_CONFIG;
    }

    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &menu->btnMP.rect))
    {
        *currentScreen = SCREEN_CONFIG;
    }

    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &menu->btnExit.rect))
    {
        *currentScreen = SCREEN_EXIT;
    }
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

//Second_screen -> SinglePlayer

int singlePlayerInit(tSinglePlayerScreen* single, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int btnWidth = BTN_W;
    int btnHeight = BTN_H;
    int logoWidth = LOGO_W;
    int logoHeight = LOGO_H;

    //logo
    //menu->logoRect = (SDL_Rect){250, 60, 300, 120};
    single->logoRect.w = logoWidth;
    single->logoRect.h = logoHeight;
    single->logoRect.x = centerX - (logoWidth / 2);
    single->logoRect.y = 10;

    //btn Back - esquina superior izquierda
    single->btnBack.rect.w = 80;
    single->btnBack.rect.h = 80;
    single->btnBack.rect.x = 20;
    single->btnBack.rect.y = 20;
    single->btnBack.state = BTN_NORMAL;

    //btn inputName
    //menu->btnSP.rect = (SDL_Rect){300,250,200,50};
    single->btnInputName.rect.w = btnWidth;
    single->btnInputName.rect.h = btnHeight;
    single->btnInputName.rect.x = centerX - (btnWidth / 2);
    single->btnInputName.rect.y = centerY + 50;

    single->btnInputName.state = BTN_NORMAL;

    //btn Continuar

    single->btnContinuar = single->btnInputName;
    single->btnContinuar.rect.y += 90;

    SDL_Color white = {255,255,255,255};

    if(lblCreate(&single->lblInputName, renderer, assets->font, "Ingresa tu nombre", white) != OK)
        return SDL_ERR;
    
    single->lblInputName.rect.x = single->btnInputName.rect.x + (single->btnInputName.rect.w - single->lblInputName.rect.w) / 2;
    single->lblInputName.rect.y = single->btnInputName.rect.y - single->lblInputName.rect.h - 10;

    if(lblCreate(&single->lblContinuar, renderer, assets->font, "Continuar", white) != OK)
        return SDL_ERR;

    single->lblContinuar.rect.x = single->btnContinuar.rect.x + (single->btnContinuar.rect.w - single->lblContinuar.rect.w) / 2;
    single->lblContinuar.rect.y = single->btnContinuar.rect.y + (single->btnContinuar.rect.h - single->lblContinuar.rect.h) / 2;

    return OK;
}

void singlePlayerUpdate(tSinglePlayerScreen* single, tInput* input, ScreenType* currentScreen)
{
    btnUpdate(&single->btnContinuar, input);

    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &single->btnBack.rect))
        *currentScreen = SCREEN_MAIN;

    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &single->btnContinuar.rect))
        *currentScreen = SCREEN_SET_CARDS;
}

void singlePlayerRender(SDL_Renderer* renderer, tSinglePlayerScreen* single, tAssets* assets)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);

    SDL_RenderCopy(renderer, assets->logo, NULL, &single->logoRect);

    SDL_Texture* tex;

    // Render Back Button (sin label, sin cambios de estado)
    SDL_RenderCopy(renderer, assets->back, NULL, &single->btnBack.rect);

    // Render InputName Button (sin cambios de estado)
    SDL_RenderCopy(renderer, assets->buttonNormal, NULL, &single->btnInputName.rect);

    // Render Input Label (encima del botón)
    SDL_RenderCopy(renderer, single->lblInputName.texture, NULL, &single->lblInputName.rect);

    // Render Continue Button (con cambios de estado)
    switch(single->btnContinuar.state)
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

    SDL_RenderCopy(renderer, tex, NULL, &single->btnContinuar.rect);
    SDL_RenderCopy(renderer, single->lblContinuar.texture, NULL, &single->lblContinuar.rect);
}

void singlePlayerDestroy(tSinglePlayerScreen* single)
{
    if(single->lblInputName.texture)
        SDL_DestroyTexture(single->lblInputName.texture);

    if(single->lblContinuar.texture)
        SDL_DestroyTexture(single->lblContinuar.texture);
}