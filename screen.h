#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include "assets.h"
#include "input.h"

#define SCREEN_TITLE "Retro-MemoTest"
#define SCREEN_WIDTH 800 //ancho
#define SCREEN_HEIGHT 600 //alto

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
}tMainMenu;

typedef enum {SCREEN_MAIN, SCREEN_GAME, SCREEN_CONFIG, SCREEN_EXIT} ScreenType;

int screenInitialize(tScreen*, const char*, int, int);
void screenShutdown(tScreen*);

int lblCreate(tLabel*, SDL_Renderer*, TTF_Font*, const char*, SDL_Color);

int mainMenuInit(tMainMenu*, SDL_Renderer*, tAssets*);
void mainMenuUpdate(tMainMenu*, tInput*, ScreenType*);
void mainMenuRender(SDL_Renderer*, tMainMenu*, tAssets*);
void mainMenuDestroy(tMainMenu*);


#endif // SCREEN_H_INCLUDED
