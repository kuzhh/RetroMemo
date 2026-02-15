#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//#define SDL_MAIN_HANDLED

#include "assets.h"
#include "screen.h"
#include "input.h"
#include "game.h"

#define OK 0
#define SDL_ERR 1

int main(int argc, char* argv[])
{
    tScreen screen = {
        .window = NULL,
        .renderer = NULL,
    };

    if(screenInitialize(&screen, SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT) != OK)
    {
        printf("Smth go wrong 'screenInitialize'.\n");
        //SDL_ERR
        screenShutdown(&screen);
        printf("Screen closed.\nSee ya!");
        return SDL_ERR;
    }

    tAssets assets;

    if(assetsLoad(&assets, screen.renderer) != OK)
    {
        printf("Smth go wrong 'assetsLoad'.\n");
        //SDL_ERR
        assetsUnload(&assets);
        screenShutdown(&screen);
        printf("Screen closed.\nSee ya!");
        return SDL_ERR;
    }

    tInput input = {0};
    input.textInputLen = 0;
    input.textActive = 0;

    //iniciar mixer
    tFormatsSnd formatsSnd = sound_start();
    if(formatsSnd == SOUND_ERR)
    {
        fprintf(stderr, "%s\n", Mix_GetError());
    }

    //Declarar board
    tBoard board = {0};
    tGame game = {0};

    tMainMenu mainMenu;
    mainMenuInit(&mainMenu, screen.renderer, &assets);

    tSinglePlayerScreen singlePlayer;
    singlePlayerInit(&singlePlayer, screen.renderer, &assets);

    tMultiplayerScreen multiPlayer;
    multiPlayerInit(&multiPlayer, screen.renderer, &assets);

    tSetCardMenu setCard;
    setCardMenuInit(&setCard, screen.renderer, &assets);

    tSetDiffMenu setDiff;
    setDiffMenuInit(&setDiff, screen.renderer, &assets);

    tPlaySPScreen playSP = {0};
    playSPInit(&playSP, screen.renderer, &assets, &game, &board, &setCard);

    tPlayMPScreen playMP = {0};
    playMPInit(&playMP, screen.renderer, &assets, &game, &board, &setCard);

    //pantalla a mostrar
    ScreenType currentScreen = SCREEN_MAIN;
    ScreenType previousScreen = SCREEN_MAIN;
    //Estado a modificar con eventos
    bool running = true;

    //Inicia reproduccion de audio infinitamente.
    sound_play(mainMenu.melody,-1);

    //OK
    printf("All good!\n");

    //Primary loop
    while(running)
    {
        running = handleEvents(&input);

        SDL_RenderClear(screen.renderer);

        ScreenType lastScreenState = currentScreen;

        //chequeo de pantallas para no iniciar la screen cada frame
        if(currentScreen != previousScreen)
        {
            if(currentScreen == SCREEN_GAME_LOW)
            {
                boardDestroy(&board);
                boardInit(&board, LOW_ROWS, LOW_COLS);
                gameInit(&game, &board, game.playerCount);
                
                if(game.playerCount == 1)
                    playSPInit(&playSP, screen.renderer, &assets, &game, &board, &setCard);
                else if(game.playerCount == 2)
                    playMPInit(&playMP, screen.renderer, &assets, &game, &board, &setCard);
            }

            if(currentScreen == SCREEN_GAME_MID)
            {
                boardDestroy(&board);
                boardInit(&board, MID_ROWS, MID_COLS);
                gameInit(&game, &board, game.playerCount);

                if(game.playerCount == 1)
                    playSPInit(&playSP, screen.renderer, &assets, &game, &board, &setCard);
                else if(game.playerCount == 2)
                    playMPInit(&playMP, screen.renderer, &assets, &game, &board, &setCard);
            }

            if(currentScreen == SCREEN_GAME_HIGH)
            {
                boardDestroy(&board);
                boardInit(&board, HIGH_ROWS, HIGH_COLS);
                gameInit(&game, &board, game.playerCount); 

                if(game.playerCount == 1)
                    playSPInit(&playSP, screen.renderer, &assets, &game, &board, &setCard);
                else if(game.playerCount == 2)
                    playMPInit(&playMP, screen.renderer, &assets, &game, &board, &setCard);
            }

            previousScreen = currentScreen;
        }

        switch(currentScreen)
        {
        case SCREEN_MAIN:
            mainMenuUpdate(&mainMenu, &input, &currentScreen, &game);
            mainMenuRender(screen.renderer, &mainMenu, &assets);
            break;
        case SCREEN_CONFIG_SINGLE:
            singlePlayerUpdate(&singlePlayer, &input, &currentScreen, &game);
            singlePlayerRender(screen.renderer, &singlePlayer, &assets, &input);
            break;
        case SCREEN_CONFIG_MULTI:
            multiPlayerUpdate(&multiPlayer, &input, &currentScreen, &game);
            multiPlayerRender(screen.renderer, &multiPlayer, &assets, &input);
            break;
        case SCREEN_SET_CARDS:
            setCardMenuUpdate(&setCard, &input, &currentScreen, previousScreen);
            setCardMenuRender(screen.renderer, &setCard, &assets);
            break;
        case SCREEN_SET_DIFFICULT:
            setDiffMenuUpdate(&setDiff, &input, &currentScreen);
            setDiffMenuRender(screen.renderer, &setDiff, &assets);
            break;
        case SCREEN_GAME_LOW:
            printf("DEBUG main: Estamos en SCREEN_GAME_LOW\n");

            if(game.playerCount == 1)
            {
                playSPUpdate(&playSP, &game, &board, &input);
                playSPRender(screen.renderer, &playSP, &assets, &board);
            }
            else if(game.playerCount == 2)
            {
                playMPUpdate(&playMP, &game, &board, &input, screen.renderer, &assets);
                playMPRender(screen.renderer, &playMP, &assets, &board, &game);
            }
            
            break;
        case SCREEN_GAME_MID:
            printf("DEBUG main: Estamos en SCREEN_GAME_MID\n");
            
            if(game.playerCount == 1)
            {
                playSPUpdate(&playSP, &game, &board, &input);
                playSPRender(screen.renderer, &playSP, &assets, &board);
            }
            else if(game.playerCount == 2)
            {
                playMPUpdate(&playMP, &game, &board, &input, screen.renderer, &assets);
                playMPRender(screen.renderer, &playMP, &assets, &board, &game);
            }

            break;
        case SCREEN_GAME_HIGH:
            printf("DEBUG main: Estamos en SCREEN_GAME_HIGH\n");
            
            if(game.playerCount == 1)
            {
                playSPUpdate(&playSP, &game, &board, &input);
                playSPRender(screen.renderer, &playSP, &assets, &board);
            }
            else if(game.playerCount == 2)
            {
                playMPUpdate(&playMP, &game, &board, &input, screen.renderer, &assets);
                playMPRender(screen.renderer, &playMP, &assets, &board, &game);
            }

            break;
        case SCREEN_GAMEOVER:
            printf("DEBUG main: Cambiando a SCREEN_GAMEOVER\n");
            running = false;
            break;
        case SCREEN_EXIT:
            printf("DEBUG main: Cambiando a SCREEN_EXIT\n");
            running = false;
            break;
        default:
            printf("DEBUG main: Screen desconocida: %d\n", currentScreen);
            break;
        }

        if (currentScreen == SCREEN_SET_CARDS && lastScreenState != SCREEN_SET_CARDS) {
            previousScreen = lastScreenState;
            printf("DEBUG: Guardado previousScreen ID: %d\n", previousScreen);
        }

        SDL_RenderPresent(screen.renderer);

        SDL_Delay(16); /*aprox 60 fps*/
    }

    //destroys
    playMPDestroy(&playMP, &game);
    playSPDestroy(&playSP);
    setDiffMenuDestroy(&setDiff);
    setCardMenuDestroy(&setCard);
    multiPlayerDestroy(&multiPlayer);
    singlePlayerDestroy(&singlePlayer);
    mainMenuDestroy(&mainMenu);
    boardDestroy(&board);
    assetsUnload(&assets);
    screenShutdown(&screen);
    printf("Screen closed.\nSee ya!");

    return 0;
}
