#include "screen.h"
#include <string.h>

// =========================================================
// FUNCIONES AUXILIARES
// =========================================================

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
    if(SDL_Init(SDL_INIT_VIDEO))
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
    if(!screen->window) return SDL_ERR;

    screen->renderer = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);
    if(!screen->renderer) return SDL_ERR;

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

// =========================================================
// MAIN MENU
// =========================================================

int mainMenuInit(tMainMenu* menu, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int btnWidth = BTN_W;
    int btnHeight = BTN_H;

    // Logo
    menu->logoRect.w = LOGO_W;
    menu->logoRect.h = LOGO_H;
    menu->logoRect.x = centerX - (LOGO_W / 2);
    menu->logoRect.y = 10;

    // Botones
    menu->btnSP.rect = (SDL_Rect){centerX - (btnWidth / 2), centerY + 50, btnWidth, btnHeight};
    menu->btnSP.state = BTN_NORMAL;

    menu->btnMP = menu->btnSP;
    menu->btnMP.rect.y += 90;

    menu->btnExit = menu->btnSP;
    menu->btnExit.rect.y += 180;

    SDL_Color white = {255,255,255,255};

    if(lblCreate(&menu->lblSP, renderer, assets->font, "Singleplayer", white) != OK) return SDL_ERR;

    menu->lblSP.rect.x = menu->btnSP.rect.x + (menu->btnSP.rect.w - menu->lblSP.rect.w)/2;
    menu->lblSP.rect.y = menu->btnSP.rect.y + (menu->btnSP.rect.h - menu->lblSP.rect.h)/2;

    if(lblCreate(&menu->lblMP, renderer, assets->font, "Multiplayer", white) != OK) return SDL_ERR;
    menu->lblMP.rect.x = menu->btnMP.rect.x + (menu->btnMP.rect.w - menu->lblMP.rect.w)/2;
    menu->lblMP.rect.y = menu->btnMP.rect.y + (menu->btnMP.rect.h - menu->lblMP.rect.h)/2;

    if(lblCreate(&menu->lblExit, renderer, assets->font, "Exit game", white) != OK) return SDL_ERR;
    menu->lblExit.rect.x = menu->btnExit.rect.x + (menu->btnExit.rect.w - menu->lblExit.rect.w)/2;
    menu->lblExit.rect.y = menu->btnExit.rect.y + (menu->btnExit.rect.h - menu->lblExit.rect.h)/2;

    return OK;
}

void mainMenuUpdate(tMainMenu* menu, tInput* input, ScreenType* currentScreen)
{
    btnUpdate(&menu->btnSP, input);
    btnUpdate(&menu->btnMP, input);
    btnUpdate(&menu->btnExit, input);

    if(input->mouseReleased)
    {
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnSP.rect)){
            *currentScreen = SCREEN_CONFIG_SINGLE;
            sound_play(menu->btnSP.melody,0,-1);
        }

        if(pointInRect(input->mouseX, input->mouseY, &menu->btnMP.rect)) {
                *currentScreen = SCREEN_CONFIG_MULTI;
                sound_play(menu->btnMP.melody,0,-1);
        }
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnExit.rect)){
                *currentScreen = SCREEN_EXIT;
                sound_play(menu->btnExit.melody,0,-1);
        }
    }
}

void mainMenuRender(SDL_Renderer* renderer, tMainMenu* menu, tAssets* assets)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);
    SDL_RenderCopy(renderer, assets->logo, NULL, &menu->logoRect);

    SDL_Texture* tex;

    // SP
    switch(menu->btnSP.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btnSP.rect);

    // MP
    switch(menu->btnMP.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btnMP.rect);

    // Exit
    switch(menu->btnExit.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btnExit.rect);

    SDL_RenderCopy(renderer, menu->lblSP.texture, NULL, &menu->lblSP.rect);
    SDL_RenderCopy(renderer, menu->lblMP.texture, NULL, &menu->lblMP.rect);
    SDL_RenderCopy(renderer, menu->lblExit.texture, NULL, &menu->lblExit.rect);
}

void mainMenuDestroy(tMainMenu* menu)
{
    if(menu->lblSP.texture) SDL_DestroyTexture(menu->lblSP.texture);
    if(menu->lblMP.texture) SDL_DestroyTexture(menu->lblMP.texture);
    if(menu->lblExit.texture) SDL_DestroyTexture(menu->lblExit.texture);
}

// =========================================================
// SINGLE PLAYER SCREEN
// =========================================================

int singlePlayerInit(tSinglePlayerScreen* single, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    single->logoRect = (SDL_Rect){centerX - (LOGO_W/2), 10, LOGO_W, LOGO_H};

    single->btnBack.rect = (SDL_Rect){20, 20, 80, 80};
    single->btnBack.state = BTN_NORMAL;

    single->btnInputName.rect = (SDL_Rect){centerX - (BTN_W / 2), centerY + 50, BTN_W, BTN_H};
    single->btnInputName.state = BTN_NORMAL;

    single->btnContinuar = single->btnInputName;
    single->btnContinuar.rect.y += 90;

    SDL_Color white = {255,255,255,255};

    if(lblCreate(&single->lblInputName, renderer, assets->font, "Ingresa tu nombre", white) != OK) return SDL_ERR;
    single->lblInputName.rect.x = single->btnInputName.rect.x + (single->btnInputName.rect.w - single->lblInputName.rect.w) / 2;
    single->lblInputName.rect.y = single->btnInputName.rect.y - single->lblInputName.rect.h - 10;

    if(lblCreate(&single->lblContinuar, renderer, assets->font, "Continuar", white) != OK) return SDL_ERR;
    single->lblContinuar.rect.x = single->btnContinuar.rect.x + (single->btnContinuar.rect.w - single->lblContinuar.rect.w) / 2;
    single->lblContinuar.rect.y = single->btnContinuar.rect.y + (single->btnContinuar.rect.h - single->lblContinuar.rect.h) / 2;

    return OK;
}

void singlePlayerUpdate(tSinglePlayerScreen* single, tInput* input, ScreenType* currentScreen, tPlayer *player)
{
    btnUpdate(&single->btnContinuar, input);

    // BACK
    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &single->btnBack.rect))
    {
        input->textActive = 0;
        SDL_StopTextInput();
        input->textInput[0] = '\0';
        input->textInputLen = 0;
        *currentScreen = SCREEN_MAIN;
        return;
    }

    // CLICK INPUT
    if(input->mouseReleased)
    {
        if(pointInRect(input->mouseX, input->mouseY, &single->btnInputName.rect))
        {
            if(!input->textActive) {
                input->textActive = 1;
                SDL_StartTextInput();
            }
        }
        else if(!pointInRect(input->mouseX, input->mouseY, &single->btnContinuar.rect))
        {
            if(input->textActive) {
                input->textActive = 0;
                SDL_StopTextInput();
            }
        }
    }

    // CONTINUAR
    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &single->btnContinuar.rect))
    {
        if (input->textInputLen > 0) strncpy(player->namePlayer, input->textInput, MAX_TEXT_INPUT - 1);
        else strcpy(player->namePlayer, "Player 1");

        player->namePlayer[MAX_TEXT_INPUT - 1] = '\0';

        input->textActive = 0;
        SDL_StopTextInput();
        input->textInput[0] = '\0';
        input->textInputLen = 0;
        *currentScreen = SCREEN_SET_CARDS;
    }
}

void singlePlayerRender(SDL_Renderer* renderer, tSinglePlayerScreen* single, tAssets* assets, tInput* input)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);
    SDL_RenderCopy(renderer, assets->logo, NULL, &single->logoRect);
    SDL_RenderCopy(renderer, assets->back, NULL, &single->btnBack.rect);

    // Input Box
    SDL_RenderCopy(renderer, assets->buttonNormal, NULL, &single->btnInputName.rect);
    SDL_RenderCopy(renderer, single->lblInputName.texture, NULL, &single->lblInputName.rect);

    // Texto Input
    if(input)
    {
        char txtBuf[MAX_TEXT_INPUT + 1];
        memset(txtBuf, 0, sizeof(txtBuf));
        if(input->textInputLen > 0) {
            strncpy(txtBuf, input->textInput, MAX_TEXT_INPUT);
            txtBuf[MAX_TEXT_INPUT] = '\0';
        }

        if(strlen(txtBuf) > 0 || input->textActive)
        {
            SDL_Color txtColor = {255,255,255,255};
            // Espacio dummy si está vacío para calcular altura
            SDL_Surface* surf = TTF_RenderText_Blended(assets->font, (strlen(txtBuf)>0)?txtBuf:" ", txtColor);
            if(surf) {
                SDL_Texture* txtTex = SDL_CreateTextureFromSurface(renderer, surf);
                if(txtTex) {
                    int tw, th;
                    SDL_QueryTexture(txtTex, NULL, NULL, &tw, &th);
                    SDL_Rect dst = single->btnInputName.rect;
                    dst.x += 50;
                    dst.y += (dst.h - th) / 2;
                    dst.w = tw;
                    dst.h = th;

                    if(strlen(txtBuf)>0) SDL_RenderCopy(renderer, txtTex, NULL, &dst);

                    if(input->textActive) {
                        int cx = dst.x + (strlen(txtBuf)>0 ? dst.w : 0) + 2;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderDrawLine(renderer, cx, dst.y, cx, dst.y + dst.h);
                    }
                    SDL_DestroyTexture(txtTex);
                }
                SDL_FreeSurface(surf);
            }
        }
    }

    // Continuar
    SDL_Texture* tex;
    switch(single->btnContinuar.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &single->btnContinuar.rect);
    SDL_RenderCopy(renderer, single->lblContinuar.texture, NULL, &single->lblContinuar.rect);
}

void singlePlayerDestroy(tSinglePlayerScreen* single)
{
    if(single->lblInputName.texture) SDL_DestroyTexture(single->lblInputName.texture);
    if(single->lblContinuar.texture) SDL_DestroyTexture(single->lblContinuar.texture);
}

// =========================================================
// MULTIPLAYER SCREEN
// =========================================================

int multiPlayerInit(tMultiplayerScreen* multi, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int btnWidth = BTN_W;
    int btnHeight = BTN_H;

    // Logos y Botones básicos
    multi->logoRect = (SDL_Rect){centerX - (LOGO_W/2), 10, LOGO_W, LOGO_H};
    multi->btnBack.rect = (SDL_Rect){20, 20, 80, 80};
    multi->btnBack.state = BTN_NORMAL;

    // Inputs Positions
    multi->btnInputName1.rect = (SDL_Rect){(centerX / 2) - (btnWidth / 2), centerY + 50, btnWidth, btnHeight};
    multi->btnInputName1.state = BTN_NORMAL;

    multi->btnInputName2.rect = (SDL_Rect){centerX + (centerX / 2) - (btnWidth / 2), centerY + 50, btnWidth, btnHeight};
    multi->btnInputName2.state = BTN_NORMAL;

    multi->btnContinuar.rect = (SDL_Rect){centerX - (btnWidth / 2), centerY + 170, btnWidth, btnHeight};
    multi->btnContinuar.state = BTN_NORMAL;

    // Labels
    SDL_Color white = {255,255,255,255};
    if(lblCreate(&multi->lblInputName1, renderer, assets->font, "Ingresa nombre de 1P", white) != OK) return SDL_ERR;
    multi->lblInputName1.rect.x = multi->btnInputName1.rect.x + (multi->btnInputName1.rect.w - multi->lblInputName1.rect.w) / 2;
    multi->lblInputName1.rect.y = multi->btnInputName1.rect.y - multi->lblInputName1.rect.h - 10;

    if(lblCreate(&multi->lblInputName2, renderer, assets->font, "Ingresa nombre de 2P", white) != OK) return SDL_ERR;
    multi->lblInputName2.rect.x = multi->btnInputName2.rect.x + (multi->btnInputName2.rect.w - multi->lblInputName2.rect.w) / 2;
    multi->lblInputName2.rect.y = multi->btnInputName2.rect.y - multi->lblInputName2.rect.h - 10;

    if(lblCreate(&multi->lblContinuar, renderer, assets->font, "Continuar", white) != OK) return SDL_ERR;
    multi->lblContinuar.rect.x = multi->btnContinuar.rect.x + (multi->btnContinuar.rect.w - multi->lblContinuar.rect.w) / 2;
    multi->lblContinuar.rect.y = multi->btnContinuar.rect.y + (multi->btnContinuar.rect.h - multi->lblContinuar.rect.h) / 2;

    // Limpieza de memoria
    memset(multi->textInput1, 0, sizeof(multi->textInput1));
    memset(multi->textInput2, 0, sizeof(multi->textInput2));
    multi->textInputLen1 = 0;
    multi->textInputLen2 = 0;
    multi->textActive1 = 0;
    multi->textActive2 = 0;

    return OK;
}

void multiPlayerUpdate(tMultiplayerScreen* multi, tInput* input, ScreenType* currentScreen, tPlayer* player)
{
    btnUpdate(&multi->btnContinuar, input);

    // BACK
    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &multi->btnBack.rect))
    {
        multi->textActive1 = 0; multi->textActive2 = 0;
        input->textActive = 0;
        SDL_StopTextInput();
        input->textInput[0] = '\0';
        input->textInputLen = 0;
        *currentScreen = SCREEN_MAIN;
        return;
    }

    // CLICKS EN INPUTS
    if(input->mouseReleased)
    {
        // 1. Click en Input 1
        if(pointInRect(input->mouseX, input->mouseY, &multi->btnInputName1.rect))
        {
            if(!multi->textActive1)
            {
                // Si venimos del 2, guardamos su estado
                if (multi->textActive2) {
                    strncpy(multi->textInput2, input->textInput, MAX_TEXT_INPUT - 1);
                    multi->textInputLen2 = (int)strlen(multi->textInput2);
                }

                multi->textActive1 = 1;
                multi->textActive2 = 0;
                input->textActive = 1;

                // Cargamos el texto del 1 al Input Global
                strncpy(input->textInput, multi->textInput1, MAX_TEXT_INPUT - 1);
                input->textInput[MAX_TEXT_INPUT - 1] = '\0';

                // CORRECCION: Recalcular longitud real para que el backspace sepa dónde está
                input->textInputLen = (int)strlen(input->textInput);

                SDL_StartTextInput();
            }
        }
        // 2. Click en Input 2
        else if(pointInRect(input->mouseX, input->mouseY, &multi->btnInputName2.rect))
        {
            if(!multi->textActive2)
            {
                // Si venimos del 1, guardamos su estado
                if (multi->textActive1) {
                    strncpy(multi->textInput1, input->textInput, MAX_TEXT_INPUT - 1);
                    multi->textInputLen1 = (int)strlen(multi->textInput1);
                }

                multi->textActive2 = 1;
                multi->textActive1 = 0;
                input->textActive = 1;

                // Cargamos el texto del 2 al Input Global
                strncpy(input->textInput, multi->textInput2, MAX_TEXT_INPUT - 1);
                input->textInput[MAX_TEXT_INPUT - 1] = '\0';

                // CORRECCION: Recalcular longitud real
                input->textInputLen = (int)strlen(input->textInput);

                SDL_StartTextInput();
            }
        }
        // 3. Click Afuera
        else if (!pointInRect(input->mouseX, input->mouseY, &multi->btnContinuar.rect))
        {
            if(multi->textActive1) {
                strncpy(multi->textInput1, input->textInput, MAX_TEXT_INPUT - 1);
                multi->textInputLen1 = (int)strlen(multi->textInput1);
            } else if (multi->textActive2) {
                strncpy(multi->textInput2, input->textInput, MAX_TEXT_INPUT - 1);
                multi->textInputLen2 = (int)strlen(multi->textInput2);
            }
            multi->textActive1 = 0; multi->textActive2 = 0;
            input->textActive = 0;
            SDL_StopTextInput();
        }
    }

    // SINCRONIZACIÓN FRAME A FRAME
    if(multi->textActive1)
    {
        strncpy(multi->textInput1, input->textInput, MAX_TEXT_INPUT - 1);
        multi->textInput1[MAX_TEXT_INPUT - 1] = '\0';
        multi->textInputLen1 = input->textInputLen;
    }
    else if(multi->textActive2)
    {
        strncpy(multi->textInput2, input->textInput, MAX_TEXT_INPUT - 1);
        multi->textInput2[MAX_TEXT_INPUT - 1] = '\0';
        multi->textInputLen2 = input->textInputLen;
    }

    // CONTINUAR
    if(input->mouseReleased && pointInRect(input->mouseX, input->mouseY, &multi->btnContinuar.rect))
    {
        if(multi->textActive1) strncpy(multi->textInput1, input->textInput, MAX_TEXT_INPUT-1);
        if(multi->textActive2) strncpy(multi->textInput2, input->textInput, MAX_TEXT_INPUT-1);

        if(strlen(multi->textInput1) > 0) strcpy(player[0].namePlayer, multi->textInput1);
        else strcpy(player[0].namePlayer, "Player 1");

        if(strlen(multi->textInput2) > 0) strcpy(player[1].namePlayer, multi->textInput2);
        else strcpy(player[1].namePlayer, "Player 2");

        multi->textActive1 = 0; multi->textActive2 = 0;
        input->textActive = 0;
        SDL_StopTextInput();
        input->textInput[0] = '\0';
        input->textInputLen = 0;
        *currentScreen = SCREEN_SET_CARDS;
    }
}

void multiPlayerRender(SDL_Renderer* renderer, tMultiplayerScreen* multi, tAssets* assets, tInput* input)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);
    SDL_RenderCopy(renderer, assets->logo, NULL, &multi->logoRect);
    SDL_RenderCopy(renderer, assets->back, NULL, &multi->btnBack.rect);

    SDL_Texture* tex;
    SDL_Color txtColor = {255,255,255,255};

    // --- INPUT 1 ---
    SDL_RenderCopy(renderer, assets->buttonNormal, NULL, &multi->btnInputName1.rect);
    SDL_RenderCopy(renderer, multi->lblInputName1.texture, NULL, &multi->lblInputName1.rect);

    // Usamos la variable local o global según corresponda
    const char* txt1 = multi->textActive1 ? input->textInput : multi->textInput1;

    if(strlen(txt1) > 0 || multi->textActive1)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(assets->font, (strlen(txt1)>0)?txt1:" ", txtColor);
        if(surf) {
            SDL_Texture* txtTex = SDL_CreateTextureFromSurface(renderer, surf);
            if(txtTex) {
                int tw, th;
                SDL_QueryTexture(txtTex, NULL, NULL, &tw, &th);
                SDL_Rect dst = multi->btnInputName1.rect;
                dst.x += 50; dst.y += (dst.h - th)/2; dst.w = tw; dst.h = th;

                if(strlen(txt1)>0) SDL_RenderCopy(renderer, txtTex, NULL, &dst);

                if(multi->textActive1) {
                    int cx = dst.x + (strlen(txt1)>0 ? dst.w : 0) + 2;
                    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                    SDL_RenderDrawLine(renderer, cx, dst.y, cx, dst.y + dst.h);
                }
                SDL_DestroyTexture(txtTex);
            }
            SDL_FreeSurface(surf);
        }
    }

    // --- INPUT 2 ---
    SDL_RenderCopy(renderer, assets->buttonNormal, NULL, &multi->btnInputName2.rect);
    SDL_RenderCopy(renderer, multi->lblInputName2.texture, NULL, &multi->lblInputName2.rect);

    const char* txt2 = multi->textActive2 ? input->textInput : multi->textInput2;
    if(strlen(txt2) > 0 || multi->textActive2)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(assets->font, (strlen(txt2)>0)?txt2:" ", txtColor);
        if(surf) {
            SDL_Texture* txtTex = SDL_CreateTextureFromSurface(renderer, surf);
            if(txtTex) {
                int tw, th;
                SDL_QueryTexture(txtTex, NULL, NULL, &tw, &th);
                SDL_Rect dst = multi->btnInputName2.rect;
                dst.x += 50; dst.y += (dst.h - th)/2; dst.w = tw; dst.h = th;

                if(strlen(txt2)>0) SDL_RenderCopy(renderer, txtTex, NULL, &dst);

                if(multi->textActive2) {
                    int cx = dst.x + (strlen(txt2)>0 ? dst.w : 0) + 2;
                    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                    SDL_RenderDrawLine(renderer, cx, dst.y, cx, dst.y + dst.h);
                }
                SDL_DestroyTexture(txtTex);
            }
            SDL_FreeSurface(surf);
        }
    }

    // Continuar
    switch(multi->btnContinuar.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &multi->btnContinuar.rect);
    SDL_RenderCopy(renderer, multi->lblContinuar.texture, NULL, &multi->lblContinuar.rect);
}

void multiPlayerDestroy(tMultiplayerScreen* multi)
{
    if(multi->lblInputName1.texture) SDL_DestroyTexture(multi->lblInputName1.texture);
    if(multi->lblInputName2.texture) SDL_DestroyTexture(multi->lblInputName2.texture);
    if(multi->lblContinuar.texture) SDL_DestroyTexture(multi->lblContinuar.texture);
}

// =========================================================
// SETCARD MENU
// =========================================================

int setCardMenuInit(tSetCardMenu* menu, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int btnWidth = BTN_W;
    int btnHeight = BTN_H;

    // Logo
    menu->logoRect = (SDL_Rect){centerX - (LOGO_W/2), 10, LOGO_W, LOGO_H};

    // Botones
    menu->btnBack.rect = (SDL_Rect){20, 20, 80, 80};

    menu->btn1S.rect = (SDL_Rect){centerX - (btnWidth / 2), centerY + 50, btnWidth, btnHeight};
    menu->btn1S.state = BTN_NORMAL;

    menu->btn2S = menu->btn1S;
    menu->btn2S.rect.y += 90;

    SDL_Color white = {255,255,255,255};

    if(lblCreate(&menu->lbl1S, renderer, assets->font, "Medieval", white) != OK) return SDL_ERR;
    menu->lbl1S.rect.x = menu->btn1S.rect.x + (menu->btn1S.rect.w - menu->lbl1S.rect.w)/2;
    menu->lbl1S.rect.y = menu->btn1S.rect.y + (menu->btn1S.rect.h - menu->lbl1S.rect.h)/2;

    if(lblCreate(&menu->lbl2S, renderer, assets->font, "Griego", white) != OK) return SDL_ERR;
    menu->lbl2S.rect.x = menu->btn2S.rect.x + (menu->btn2S.rect.w - menu->lbl2S.rect.w)/2;
    menu->lbl2S.rect.y = menu->btn2S.rect.y + (menu->btn2S.rect.h - menu->lbl2S.rect.h)/2;

    return OK;
}

void setCardMenuUpdate(tSetCardMenu* menu, tInput* input, ScreenType* currentScreen, ScreenType PreviousScreen)
{
    btnUpdate(&menu->btn1S, input);
    btnUpdate(&menu->btn2S, input);
    btnUpdate(&menu->btnBack, input);

    if(input->mouseReleased)
    {
        if(pointInRect(input->mouseX, input->mouseY, &menu->btn1S.rect))
        {
            strcpy(menu->setCardChoosen, "Medieval");
            *currentScreen = SCREEN_SET_DIFFICULT;
            return;
        }
        if(pointInRect(input->mouseX, input->mouseY, &menu->btn2S.rect))
        {
            strcpy(menu->setCardChoosen, "Griego");
            *currentScreen = SCREEN_SET_DIFFICULT;
            return;
        }
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnBack.rect))
            *currentScreen = PreviousScreen;
    }
}

void setCardMenuRender(SDL_Renderer* renderer, tSetCardMenu* menu, tAssets* assets)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);
    SDL_RenderCopy(renderer, assets->logo, NULL, &menu->logoRect);
    // Back button
    SDL_RenderCopy(renderer, assets->back, NULL, &menu->btnBack.rect);

    SDL_Texture* tex;

    // 1S
    switch(menu->btn1S.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btn1S.rect);

    // 2S
    switch(menu->btn2S.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btn2S.rect);

    SDL_RenderCopy(renderer, menu->lbl1S.texture, NULL, &menu->lbl1S.rect);
    SDL_RenderCopy(renderer, menu->lbl2S.texture, NULL, &menu->lbl2S.rect);
}

void setCardMenuDestroy(tSetCardMenu* menu)
{
    if(menu->lbl1S.texture) SDL_DestroyTexture(menu->lbl1S.texture);
    if(menu->lbl2S.texture) SDL_DestroyTexture(menu->lbl2S.texture);
}

// =========================================================
// DIFF MENU
// =========================================================

int setDiffMenuInit(tSetDiffMenu* menu, SDL_Renderer* renderer, tAssets* assets)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int btnWidth = BTN_W;
    int btnHeight = BTN_H;

    // Logo
    menu->logoRect.w = LOGO_W;
    menu->logoRect.h = LOGO_H;
    menu->logoRect.x = centerX - (LOGO_W / 2);
    menu->logoRect.y = 10;

    // Botones
    menu->btnBack.rect = (SDL_Rect){20, 20, 80, 80};
    menu->btnBack.state = BTN_NORMAL;

    menu->btnLow.rect = (SDL_Rect){centerX - (btnWidth / 2), centerY + 50, btnWidth, btnHeight};
    menu->btnLow.state = BTN_NORMAL;

    menu->btnMid = menu->btnLow;
    menu->btnMid.rect.y += 90;

    menu->btnHigh = menu->btnLow;
    menu->btnHigh.rect.y += 180;

    SDL_Color white = {255,255,255,255};

    if(lblCreate(&menu->lblLow, renderer, assets->font, "3x4", white) != OK) return SDL_ERR;
    menu->lblLow.rect.x = menu->btnLow.rect.x + (menu->btnLow.rect.w - menu->lblLow.rect.w)/2;
    menu->lblLow.rect.y = menu->btnLow.rect.y + (menu->btnLow.rect.h - menu->lblLow.rect.h)/2;

    if(lblCreate(&menu->lblMid, renderer, assets->font, "4x4", white) != OK) return SDL_ERR;
    menu->lblMid.rect.x = menu->btnMid.rect.x + (menu->btnMid.rect.w - menu->lblMid.rect.w)/2;
    menu->lblMid.rect.y = menu->btnMid.rect.y + (menu->btnMid.rect.h - menu->lblMid.rect.h)/2;

    if(lblCreate(&menu->lblHigh, renderer, assets->font, "4x5", white) != OK) return SDL_ERR;
    menu->lblHigh.rect.x = menu->btnHigh.rect.x + (menu->btnHigh.rect.w - menu->lblHigh.rect.w)/2;
    menu->lblHigh.rect.y = menu->btnHigh.rect.y + (menu->btnHigh.rect.h - menu->lblHigh.rect.h)/2;


    return OK;
}

void setDiffMenuUpdate(tSetDiffMenu* menu, tInput* input, ScreenType* currentScreen)
{
    btnUpdate(&menu->btnLow, input);
    btnUpdate(&menu->btnMid, input);
    btnUpdate(&menu->btnHigh, input);
    btnUpdate(&menu->btnBack, input);

    if(input->mouseReleased)
    {
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnLow.rect))
        {
            *currentScreen = SCREEN_GAME_LOW;
            return;
        }
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnMid.rect))
        {
            *currentScreen = SCREEN_GAME_MID;
            return;
        }
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnHigh.rect))
        {
            *currentScreen = SCREEN_GAME_HIGH;
            return;
        }
        if(pointInRect(input->mouseX, input->mouseY, &menu->btnBack.rect)) *currentScreen = SCREEN_SET_CARDS;
    }
}

void setDiffMenuRender(SDL_Renderer* renderer, tSetDiffMenu* menu, tAssets* assets)
{
    SDL_RenderCopy(renderer, assets->background, NULL, NULL);
    SDL_RenderCopy(renderer, assets->logo, NULL, &menu->logoRect);

    SDL_Texture* tex;

    // Low
    switch(menu->btnLow.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btnLow.rect);

    // Mid
    switch(menu->btnMid.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btnMid.rect);

    // High
    switch(menu->btnHigh.state) {
        case BTN_NORMAL: tex = assets->buttonNormal; break;
        case BTN_HOVER: tex = assets->buttonHover; break;
        case BTN_PRESSED: tex = assets->buttonPressed; break;
        default: tex = assets->buttonNormal; break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &menu->btnHigh.rect);

    // Back button
    SDL_RenderCopy(renderer, assets->back, NULL, &menu->btnBack.rect);

    SDL_RenderCopy(renderer, menu->lblLow.texture, NULL, &menu->lblLow.rect);
    SDL_RenderCopy(renderer, menu->lblMid.texture, NULL, &menu->lblMid.rect);
    SDL_RenderCopy(renderer, menu->lblHigh.texture, NULL, &menu->lblHigh.rect);
}

void setDiffMenuDestroy(tSetDiffMenu* menu)
{
    if(menu->lblLow.texture) SDL_DestroyTexture(menu->lblLow.texture);
    if(menu->lblMid.texture) SDL_DestroyTexture(menu->lblMid.texture);
    if(menu->lblHigh.texture) SDL_DestroyTexture(menu->lblHigh.texture);
}
