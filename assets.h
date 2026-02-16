#ifndef ASSETS_H_INCLUDED
#define ASSETS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "input.h"
#include "sound.h"

#define OK 0
#define SDL_ERR 1

#define MAX_CARD_TYPES 10

typedef enum {BTN_NORMAL, BTN_HOVER, BTN_PRESSED} BtnState;
//typedef enum {FONT_H1, FONT_H2, FONT_H3, FONT_COUNT} FontType;
typedef enum {CSET_DS, CSET_GREEK} CardSetType;

// --- DEFINES DE ASSETS ---
#define FONT_PATCH "fnt/PixelifySans-VariableFont_wght.ttf"
#define FONT_SIZE 24

#define BACKGROUND_FIRST "img/background1.png"
#define BACKGROUND_SECOND "img/background2.png"
#define LOGO "img/logo.png"
#define BUTTON_NORMAL "img/btn_normal.png"
#define BUTTON_HOVER "img/btn_hover.png"
#define BUTTON_PRESSED "img/btn_normal.png"
#define BUTTON_BACK "img/back.png"
#define SCORE_BOX "img/score_box.png"

// Set 1 (Dark Souls)
#define SET_CARD_BACK_1 "img/ds_back.png"
#define SET_CARD_DS_1 "img/ds_card1.png"
#define SET_CARD_DS_2 "img/ds_card2.png"
#define SET_CARD_DS_3 "img/ds_card3.png"
#define SET_CARD_DS_4 "img/ds_card4.png"
#define SET_CARD_DS_5 "img/ds_card5.png"
#define SET_CARD_DS_6 "img/ds_card6.png"
#define SET_CARD_DS_7 "img/ds_card7.png"
#define SET_CARD_DS_8 "img/ds_card8.png"
#define SET_CARD_DS_9 "img/ds_card9.png"
#define SET_CARD_DS_10 "img/ds_card10.png"

// Set 2 (Griego)
#define SET_CARD_BACK_2 "img/greek_back.png"
#define SET_CARD_GR_1 "img/greek_card1.png"
#define SET_CARD_GR_2 "img/greek_card2.png"
#define SET_CARD_GR_3 "img/greek_card3.png"
#define SET_CARD_GR_4 "img/greek_card4.png"
#define SET_CARD_GR_5 "img/greek_card5.png"
#define SET_CARD_GR_6 "img/greek_card6.png"
#define SET_CARD_GR_7 "img/greek_card7.png"
#define SET_CARD_GR_8 "img/greek_card8.png"
#define SET_CARD_GR_9 "img/greek_card9.png"
#define SET_CARD_GR_10 "img/greek_card10.png"

// Arrays de paths para cartas
static const char* ds_card_paths[MAX_CARD_TYPES] = {
    SET_CARD_DS_1, SET_CARD_DS_2, SET_CARD_DS_3, SET_CARD_DS_4, SET_CARD_DS_5,
    SET_CARD_DS_6, SET_CARD_DS_7, SET_CARD_DS_8, SET_CARD_DS_9, SET_CARD_DS_10
};

static const char* greek_card_paths[MAX_CARD_TYPES] = {
    SET_CARD_GR_1, SET_CARD_GR_2, SET_CARD_GR_3, SET_CARD_GR_4, SET_CARD_GR_5,
    SET_CARD_GR_6, SET_CARD_GR_7, SET_CARD_GR_8, SET_CARD_GR_9, SET_CARD_GR_10
};

/*typedef struct
{
    TTF_Font* fonts[FONT_COUNT];
} tFonts;*/

typedef struct
{
    SDL_Rect rect;
    SDL_Texture* normal;
    SDL_Texture* hover;
    SDL_Texture* pressed;
    BtnState state;
    tSound *melody;
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
    SDL_Texture* backgroundGame;

    SDL_Texture* scoreBox;

    SDL_Texture* logo;

    SDL_Texture* back;

    TTF_Font* font;
}tAssets;

int cardsetLoad(tCardSet* cardSet, SDL_Renderer*, CardSetType type);
void cardsetUnload(tCardSet* cardSet);

int assetsLoad(tAssets* assets, SDL_Renderer*);
void assetsUnload(tAssets* assets);

void btnUpdate(tButton* btn, tInput* input);


#endif // ASSETS_H_INCLUDED
