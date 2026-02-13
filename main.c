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

    //pantalla a mostrar
    ScreenType currentScreen = SCREEN_MAIN;
    //Estado a modificar con eventos
    bool running = true;

    tInput input = {0};
    input.textInputLen = 0;

    //OK
    printf("All good!\n");

    //Primary loop
    while(running)
    {
        running = handleEvents(&input);

        SDL_RenderClear(screen.renderer);

        switch(currentScreen)
        {
        case SCREEN_MAIN:
            mainMenuUpdate(&mainMenu, &input, &currentScreen);
            mainMenuRender(screen.renderer, &mainMenu, &assets);
            break;
        case SCREEN_CONFIG:
            singlePlayerUpdate(&singlePlayer, &input, &currentScreen);
            singlePlayerRender(screen.renderer, &singlePlayer, &assets);
            break;
        case SCREEN_SET_CARDS:
            printf("DEBUG main: Estamos en SCREEN_SET_CARDS\n");
            break;
        case SCREEN_GAME:
            printf("DEBUG main: Estamos en SCREEN_GAME\n");
            /*...*/
            break;
        case SCREEN_EXIT:
            printf("DEBUG main: Cambiando a SCREEN_EXIT\n");
            running = false;
            break;
        default:
            printf("DEBUG main: Screen desconocida: %d\n", currentScreen);
            break;
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
