#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include "sound.h"
#include "assets.h"
#include "input.h"
#include "game.h"

#define SCREEN_TITLE "Retro-MemoTest"
#define SCREEN_WIDTH 1366 //ancho
#define SCREEN_HEIGHT 768 //alto

#define LOGO_W 400
#define LOGO_H 400

#define BTN_W 436
#define BTN_H 90

#define IMG_FLAGS IMG_INIT_PNG

#define OK 0
#define SDL_ERR 1

typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
} tScreen;

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
} tLabel;

typedef struct{
    tButton btnSP;
    tButton btnMP;
    tButton btnExit;

    tLabel lblSP;
    tLabel lblMP;
    tLabel lblExit;

    SDL_Rect logoRect;

    tSound *melody;
} tMainMenu;

typedef struct{
    tButton btn1S;
    tButton btn2S;
    tButton btnBack;

    tLabel lbl1S;
    tLabel lbl2S;

    char setCardChoosen[MAX_TEXT_INPUT];

    SDL_Rect logoRect;
} tSetCardMenu;

typedef struct{
    tButton btnLow;
    tButton btnMid;
    tButton btnHigh;
    tButton btnBack;

    tLabel lblLow;
    tLabel lblMid;
    tLabel lblHigh;

    SDL_Rect logoRect;
} tSetDiffMenu;

typedef struct{
    tButton btnInputName;
    tButton btnBack;
    tButton btnContinuar;

    tLabel lblInputName;
    tLabel lblBack;
    tLabel lblContinuar;

    SDL_Rect logoRect;
} tSinglePlayerScreen;

typedef struct{
    tButton btnInputName1;
    tButton btnInputName2;
    tButton btnBack;
    tButton btnContinuar;

    tLabel lblInputName1;
    tLabel lblInputName2;
    tLabel lblBack;
    tLabel lblContinuar;

    SDL_Rect logoRect;
    char textInput1[MAX_TEXT_INPUT];
    int textInputLen1;
    int textActive1;
    char textInput2[MAX_TEXT_INPUT];
    int textInputLen2;
    int textActive2;
} tMultiplayerScreen;

typedef enum {
    SCREEN_MAIN,
    SCREEN_CONFIG_SINGLE,
    SCREEN_CONFIG_MULTI,
    SCREEN_SET_CARDS,
    SCREEN_SET_DIFFICULT,
    SCREEN_GAME_LOW,
    SCREEN_GAME_MID,
    SCREEN_GAME_HIGH,
    SCREEN_GAMEOVER,
    SCREEN_EXIT
} ScreenType;

int screenInitialize(tScreen*, const char*, int, int);
void screenShutdown(tScreen*);

int lblCreate(tLabel*, SDL_Renderer*, TTF_Font*, const char*, SDL_Color);

int mainMenuInit(tMainMenu*, SDL_Renderer*, tAssets*);
void mainMenuUpdate(tMainMenu*, tInput*, ScreenType*);
void mainMenuRender(SDL_Renderer*, tMainMenu*, tAssets*);
void mainMenuDestroy(tMainMenu*);

int singlePlayerInit(tSinglePlayerScreen*, SDL_Renderer*, tAssets*);
void singlePlayerUpdate(tSinglePlayerScreen*, tInput*, ScreenType*, tPlayer*);
void singlePlayerRender(SDL_Renderer*, tSinglePlayerScreen*, tAssets*, tInput*);
void singlePlayerDestroy(tSinglePlayerScreen*);

int multiPlayerInit(tMultiplayerScreen*, SDL_Renderer*, tAssets*);
void multiPlayerUpdate(tMultiplayerScreen*, tInput*, ScreenType*, tPlayer*);
void multiPlayerRender(SDL_Renderer*, tMultiplayerScreen*, tAssets*, tInput*);
void multiPlayerDestroy(tMultiplayerScreen*);

int setCardMenuInit(tSetCardMenu*, SDL_Renderer*, tAssets*);
void setCardMenuUpdate(tSetCardMenu*, tInput*, ScreenType*, ScreenType);
void setCardMenuRender(SDL_Renderer*, tSetCardMenu*, tAssets*);
void setCardMenuDestroy(tSetCardMenu*);

int setDiffMenuInit(tSetDiffMenu*, SDL_Renderer*, tAssets*);
void setDiffMenuUpdate(tSetDiffMenu*, tInput*, ScreenType*);
void setDiffMenuRender(SDL_Renderer*, tSetDiffMenu*, tAssets*);
void setDiffMenuDestroy(tSetDiffMenu*);

#endif // SCREEN_H_INCLUDED
