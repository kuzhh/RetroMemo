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

    //pantalla a mostrar
    ScreenType currentScreen = SCREEN_MAIN;
    ScreenType previousScreen = SCREEN_MAIN;
    //Estado a modificar con eventos
    bool running = true;

    tInput input = {0};
    input.textInputLen = 0;
    input.textActive = 0;

    // Iniciar player
    tPlayer player[2] = {{1, 0, 0, 0, "1P"},{2, 0, 0, 0,"1P"}};

    //OK
    printf("All good!\n");

    //Primary loop
    while(running)
    {
        running = handleEvents(&input);

        SDL_RenderClear(screen.renderer);

        ScreenType lastScreenState = currentScreen;

        switch(currentScreen)
        {
        case SCREEN_MAIN:
            mainMenuUpdate(&mainMenu, &input, &currentScreen);
            mainMenuRender(screen.renderer, &mainMenu, &assets);
            break;
        case SCREEN_CONFIG_SINGLE:
            singlePlayerUpdate(&singlePlayer, &input, &currentScreen, &player[0]);
            singlePlayerRender(screen.renderer, &singlePlayer, &assets, &input);
            break;
        case SCREEN_CONFIG_MULTI:
            multiPlayerUpdate(&multiPlayer, &input, &currentScreen, &player[0]);
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
            /*...*/
            break;
        case SCREEN_GAME_MID:
            printf("DEBUG main: Estamos en SCREEN_GAME_MID\n");
            /*...*/
            break;
        case SCREEN_GAME_HIGH:
            printf("DEBUG main: Estamos en SCREEN_GAME_HIGH\n");
            /*...*/
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
    mainMenuDestroy(&mainMenu);
    singlePlayerDestroy(&singlePlayer);
    assetsUnload(&assets);
    screenShutdown(&screen);
    printf("Screen closed.\nSee ya!");

    return 0;
}
