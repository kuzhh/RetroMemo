#ifndef ASSETS_H_INCLUDED
#define ASSETS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "input.h"

#define OK 0
#define SDL_ERR 1

#define MAX_CARD_TYPES 10

typedef enum {BTN_NORMAL, BTN_HOVER, BTN_PRESSED} BtnState;
typedef enum {FONT_H1, FONT_H2, FONT_H3, FONT_COUNT} FontType;
typedef enum {CSET_DS, CSET_GREEK} CardSetType;

typedef struct
{
    TTF_Font* fonts[FONT_COUNT];
} tFonts;

typedef struct
{
    SDL_Rect rect;
    SDL_Texture* normal;
    SDL_Texture* hover;
    SDL_Texture* pressed;
    BtnState state;
} tButton;

typedef struct
{
    SDL_Texture* cardFront[MAX_CARD_TYPES];
    SDL_Texture* cardBack;
}tCardSet;

typedef struct
{
    tCardSet dsSet;
    tCardSet greekSet;

    SDL_Texture* buttonNormal;
    SDL_Texture* buttonHover;
    SDL_Texture* buttonPressed;

    SDL_Texture* background;

    SDL_Texture* logo;

    TTF_Font* font;
}tAssets;

int cardsetLoad(tCardSet* cardSet, SDL_Renderer*, CardSetType type);
void cardsetUnload(tCardSet* cardSet);

int assetsLoad(tAssets* assets, SDL_Renderer*);
void assetsUnload(tAssets* assets);

void btnUpdate(tButton* btn, tInput* input);


#endif // ASSETS_H_INCLUDED
