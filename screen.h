#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "assets.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include <stdio.h>

#define SCREEN_TITLE "Retro-MemoTest"
#define SCREEN_WIDTH 1366 // ancho
#define SCREEN_HEIGHT 768 // alto

#define LOGO_W 400
#define LOGO_H 400

#define BTN_W 436
#define BTN_H 90

#define SCR_W 600
#define SCR_H 450

#define IMG_FLAGS IMG_INIT_PNG

#define OK 0
#define SDL_ERR 1

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} tScreen;

typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture;
} tLabel;

typedef struct {
    tButton btnSP;
    tButton btnMP;
    tButton btnExit;

    tLabel lblSP;
    tLabel lblMP;
    tLabel lblExit;

    SDL_Rect logoRect;

    tSound *melody;
} tMainMenu;

typedef struct {
    tButton btn1S;
    tButton btn2S;
    tButton btnBack;

    tLabel lbl1S;
    tLabel lbl2S;

    char setCardChoosen[MAX_TEXT_INPUT];

    SDL_Rect logoRect;
} tSetCardMenu;

typedef struct {
    tButton btnLow;
    tButton btnMid;
    tButton btnHigh;
    tButton btnBack;

    tLabel lblLow;
    tLabel lblMid;
    tLabel lblHigh;

    SDL_Rect logoRect;
} tSetDiffMenu;

typedef struct {
    tButton btnInputName;
    tButton btnBack;
    tButton btnContinuar;

    tLabel lblInputName;
    tLabel lblBack;
    tLabel lblContinuar;

    SDL_Rect logoRect;
} tSinglePlayerScreen;

typedef struct {
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

typedef struct {
    tButton btnBack;

    tLabel lblPlayerName;
    tLabel lblPlayerScore;
    tLabel lblBack;

    tSelection selection;
    tCardSet *activeSet;

    int scoreValue;    // score “actual” copiado desde game en Update
    int scoreRendered; // último score que se renderizó (para detectar cambios)
} tPlaySPScreen;

typedef struct {
    tButton btnBack;
    tLabel lblBack;

    tLabel lblPlayerName[MAX_PLAYERS];
    tLabel lblPlayerScore[MAX_PLAYERS];

    tSelection selection;
    tCardSet *activeSet;

    int activePlayer;   // 0 = player1, 1 = player2
    int lastPlayer;
    int scoreValue[2];
    int scoreRendered[2];
} tPlayMPScreen;

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

typedef struct {
    tButton btnTryAgain;
    tButton btnPantallaInicio;
    tButton btnExit;

    tLabel lblPlayerName;
    tLabel lblPlayerScore;
    tLabel lblPlayerMoves;
    tLabel lblTryAgain;
    tLabel lblPantallaInicio;
    tLabel lblExit;

    tSelection selection;
    tCardSet *activeSet;

    SDL_Rect logoRect;
    SDL_Rect scoreBoxRect;

    int scoreValue;    // score “actual” copiado desde game en Update
    int scoreRendered; // último score que se renderizó (para detectar cambios)

    ScreenType currentDifficulty; // dificultad actual para "Volver a jugar"
} tPlaySPScreenExit;

typedef struct {
    tButton btnTryAgain;
    tButton btnPantallaInicio;
    tButton btnExit;

    tLabel lblTryAgain;
    tLabel lblPantallaInicio;
    tLabel lblExit;

    tLabel lblP1Name;  
    tLabel lblP1Score;  
    tLabel lblP1Moves;
    tLabel lblP2Name; 
    tLabel lblP2Score;
    tLabel lblP2Moves;

    tSelection selection;
    tCardSet *activeSet;

    SDL_Rect logoRect;
    SDL_Rect scoreBoxRectP1;
    SDL_Rect scoreBoxRectP2;

    int scoreValue;    // score “actual” copiado desde game en Update
    int scoreRendered; // último score que se renderizó (para detectar cambios)

    ScreenType currentDifficulty; // dificultad actual para "Volver a jugar"
} tPlayMPScreenExit;

int screenInitialize(tScreen *, const char *, int, int);
void screenShutdown(tScreen *);
int lblCreate(tLabel *, SDL_Renderer *, TTF_Font *, const char *, SDL_Color);

int mainMenuInit(tMainMenu *, SDL_Renderer *, tAssets *);
void mainMenuUpdate(tMainMenu *, tInput *, ScreenType *, tGame *);
void mainMenuRender(SDL_Renderer *, tMainMenu *, tAssets *);
void mainMenuDestroy(tMainMenu *);

int singlePlayerInit(tSinglePlayerScreen *, SDL_Renderer *, tAssets *);
void singlePlayerUpdate(tSinglePlayerScreen *, tInput *, ScreenType *, tGame *);
void singlePlayerRender(SDL_Renderer *, tSinglePlayerScreen *, tAssets *,
                        tInput *);
void singlePlayerDestroy(tSinglePlayerScreen *);

int multiPlayerInit(tMultiplayerScreen *, SDL_Renderer *, tAssets *);
void multiPlayerUpdate(tMultiplayerScreen *, tInput *, ScreenType *, tGame *);
void multiPlayerRender(SDL_Renderer *, tMultiplayerScreen *, tAssets *,
                       tInput *);
void multiPlayerDestroy(tMultiplayerScreen *);

int setCardMenuInit(tSetCardMenu *, SDL_Renderer *, tAssets *);
void setCardMenuUpdate(tSetCardMenu *, tInput *, ScreenType *, ScreenType);
void setCardMenuRender(SDL_Renderer *, tSetCardMenu *, tAssets *);
void setCardMenuDestroy(tSetCardMenu *);

int setDiffMenuInit(tSetDiffMenu *, SDL_Renderer *, tAssets *);
void setDiffMenuUpdate(tSetDiffMenu *, tInput *, ScreenType *);
void setDiffMenuRender(SDL_Renderer *, tSetDiffMenu *, tAssets *);
void setDiffMenuDestroy(tSetDiffMenu *);

int playSPInit(tPlaySPScreen *, SDL_Renderer *, tAssets *, tGame *, tBoard *,
               tSetCardMenu *);
void playSPUpdate(tPlaySPScreen *, tGame *, tBoard *, tInput *, ScreenType *);
void playSPRender(SDL_Renderer *, tPlaySPScreen *, tAssets *, tBoard *, tInput *);
void playSPDestroy(tPlaySPScreen *);

int playMPInit(tPlayMPScreen *, SDL_Renderer *, tAssets *, tGame *, tBoard *,
               tSetCardMenu *);
void playMPUpdate(tPlayMPScreen *, tGame *, tBoard *, tInput *, SDL_Renderer *,
                  tAssets *, ScreenType *);
void playMPRender(SDL_Renderer *, tPlayMPScreen *, tAssets *, tBoard *,
                  tGame *, tInput *);
void playMPDestroy(tPlayMPScreen *, tGame *);

int playSPExitInit(tPlaySPScreenExit *, SDL_Renderer *, tAssets *, tGame *,
                   tBoard *, tSetCardMenu *, ScreenType);
void playSPExitUpdate(tPlaySPScreenExit *, tGame *, tBoard *, tInput *,
                      ScreenType *);
void playSPExitRender(SDL_Renderer *, tPlaySPScreenExit *, tAssets *, tBoard *);
void playSPExitDestroy(tPlaySPScreenExit *);

int playMPExitInit(tPlayMPScreenExit *, SDL_Renderer *, tAssets *, tGame *,
                   tBoard *, tSetCardMenu *, ScreenType);
void playMPExitUpdate(tPlayMPScreenExit *, tGame *, tBoard *, tInput *,
                      ScreenType *);
void playMPExitRender(SDL_Renderer *, tPlayMPScreenExit *, tAssets *, tBoard *);
void playMPExitDestroy(tPlayMPScreenExit *);

#endif // SCREEN_H_INCLUDED
