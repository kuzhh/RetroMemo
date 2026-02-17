/*============================================
=                 RETRO-MEMO TEST
= INTEGRANTES
=
= Apellido: Castillo, Rodrigo Nahuel
= DNI: 44258511
= Entrega: Si
= -------------
= Apellido: Villalba, Lucas Daniel
= DNI: 44413521
= Entrega: Si
= -------------
= Apellido: Scarpino, Maria Victoria
= DNI: 44816746
= Entrega: Si
= -------------
= Apellido: Gahona Ramirez, Antonella Patricia
= DNI: 45928557
= Entrega: Si
==============================================*/

#include "library.h"

#include "archive.h"
#include "assets.h"
#include "game.h"
#include "input.h"
#include "screen.h"
#include "settings.h"

int main(int argc, char *argv[]) {
  tScreen screen = {
      .window = NULL,
      .renderer = NULL,
  };

  if (screenInitialize(&screen, SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT) !=
      OK) {
    printf("Smth go wrong 'screenInitialize'.\n");
    // SDL_ERR
    screenShutdown(&screen);
    printf("Pantalla cerrada.\n¡Hasta luego!");
    return SDL_ERR;
  }

  tAssets assets;

  if (assetsLoad(&assets, screen.renderer) != OK) {
    printf("Smth go wrong 'assetsLoad'.\n");
    // SDL_ERR
    assetsUnload(&assets);
    screenShutdown(&screen);
    printf("Pantalla cerrada.\n¡Hasta luego!");
    return SDL_ERR;
  }

  tInput input = {0};
  input.textInputLen = 0;
  input.textActive = 0;

  // iniciar mixer
  tFormatsSnd formatsSnd = sound_start();
  if (formatsSnd == SOUND_ERR) {
    fprintf(stderr, "%s\n", Mix_GetError());
  }

  tSettings settings;
  settingsLoad(&settings);
  settingsApply(&settings);

  // Declarar board
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

  tPlaySPScreenExit playSPExit = {0};

  tPlayMPScreenExit playMPExit = {0};

  tSettingsScreen settingsScreen;
  settingsScreenInit(&settingsScreen, screen.renderer, &assets, &settings);

  // pantalla a mostrar
  ScreenType currentScreen = SCREEN_MAIN;
  ScreenType previousScreen = SCREEN_MAIN;
  ScreenType configSource =
      SCREEN_MAIN; // Rastrear si venimos de "single" o de "multi"
  // Estado a modificar con eventos
  bool running = true;

  // OK
  printf("All good!\n");

  // Primary loop
  while (running) {
    running = handleEvents(&input);

    SDL_RenderClear(screen.renderer);

    ScreenType lastScreenState = currentScreen;

    // chequeo de pantallas para no iniciar la screen cada frame
    if (currentScreen != previousScreen) {
      // ===== Recargar Main Menu si venimos de GAMEOVER (para refrescar TOP)
      if (currentScreen == SCREEN_MAIN && previousScreen == SCREEN_GAMEOVER) {
        mainMenuDestroy(&mainMenu);
        mainMenuInit(&mainMenu, screen.renderer, &assets);
      }

      if (currentScreen == SCREEN_GAME_LOW) {
        boardDestroy(&board);
        boardInit(&board, LOW_ROWS, LOW_COLS);
        gameInit(&game, &board, game.playerCount);

        if (game.playerCount == 1)
          playSPInit(&playSP, screen.renderer, &assets, &game, &board,
                     &setCard);
        else if (game.playerCount == 2)
          playMPInit(&playMP, screen.renderer, &assets, &game, &board,
                     &setCard);
      }

      if (currentScreen == SCREEN_GAME_MID) {
        boardDestroy(&board);
        boardInit(&board, MID_ROWS, MID_COLS);
        gameInit(&game, &board, game.playerCount);

        if (game.playerCount == 1)
          playSPInit(&playSP, screen.renderer, &assets, &game, &board,
                     &setCard);
        else if (game.playerCount == 2)
          playMPInit(&playMP, screen.renderer, &assets, &game, &board,
                     &setCard);
      }

      if (currentScreen == SCREEN_GAME_HIGH) {
        boardDestroy(&board);
        boardInit(&board, HIGH_ROWS, HIGH_COLS);
        gameInit(&game, &board, game.playerCount);

        if (game.playerCount == 1)
          playSPInit(&playSP, screen.renderer, &assets, &game, &board,
                     &setCard);
        else if (game.playerCount == 2)
          playMPInit(&playMP, screen.renderer, &assets, &game, &board,
                     &setCard);
      }

      if (currentScreen == SCREEN_GAMEOVER) {
        if (configSource == SCREEN_CONFIG_SINGLE) {
          playSPExitDestroy(&playSPExit);
          playSPExitInit(&playSPExit, screen.renderer, &assets, &game, &board,
                         &setCard, previousScreen);
          guardarRegistro(&game.players[0]);
        } else {
          playMPExitDestroy(&playMPExit);
          playMPExitInit(&playMPExit, screen.renderer, &assets, &game, &board,
                         &setCard, previousScreen);
          guardarRegistro(&game.players[0]);
          guardarRegistro(&game.players[1]);
        }
      }

      if (currentScreen == SCREEN_SETTINGS) {
        settingsScreenInit(&settingsScreen, screen.renderer, &assets,
                           &settings);
      }

      previousScreen = currentScreen;
    }

    switch (currentScreen) {
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
      setCardMenuUpdate(&setCard, &input, &currentScreen, configSource);
      setCardMenuRender(screen.renderer, &setCard, &assets);
      break;

    case SCREEN_SET_DIFFICULT:
      setDiffMenuUpdate(&setDiff, &input, &currentScreen);
      setDiffMenuRender(screen.renderer, &setDiff, &assets);
      break;

    case SCREEN_GAME_LOW:
      // printf("DEBUG main: Estamos en SCREEN_GAME_LOW\n");

      if (game.playerCount == 1) {
        playSPUpdate(&playSP, &game, &board, &input, &currentScreen);
        playSPRender(screen.renderer, &playSP, &assets, &board, &input);
      } else if (game.playerCount == 2) {
        playMPUpdate(&playMP, &game, &board, &input, screen.renderer, &assets,
                     &currentScreen);
        playMPRender(screen.renderer, &playMP, &assets, &board, &game, &input);
      }
      break;

    case SCREEN_GAME_MID:
      // printf("DEBUG main: Estamos en SCREEN_GAME_MID\n");

      if (game.playerCount == 1) {
        playSPUpdate(&playSP, &game, &board, &input, &currentScreen);
        playSPRender(screen.renderer, &playSP, &assets, &board, &input);
      } else if (game.playerCount == 2) {
        playMPUpdate(&playMP, &game, &board, &input, screen.renderer, &assets,
                     &currentScreen);
        playMPRender(screen.renderer, &playMP, &assets, &board, &game, &input);
      }
      break;

    case SCREEN_GAME_HIGH:
      // printf("DEBUG main: Estamos en SCREEN_GAME_HIGH\n");

      if (game.playerCount == 1) {
        playSPUpdate(&playSP, &game, &board, &input, &currentScreen);
        playSPRender(screen.renderer, &playSP, &assets, &board, &input);
      } else if (game.playerCount == 2) {
        playMPUpdate(&playMP, &game, &board, &input, screen.renderer, &assets,
                     &currentScreen);
        playMPRender(screen.renderer, &playMP, &assets, &board, &game, &input);
      }
      break;

    case SCREEN_GAMEOVER:
      if (configSource == SCREEN_CONFIG_SINGLE) {
        // printf("DEBUG main: Cambiando a SCREEN_GAMEOVER\n");
        playSPExitUpdate(&playSPExit, &game, &board, &input, &currentScreen);
        playSPExitRender(screen.renderer, &playSPExit, &assets, &board);
      } else {
        // printf("DEBUG main: Cambiando a SCREEN_GAMEOVER\n");
        playMPExitUpdate(&playMPExit, &game, &board, &input, &currentScreen);
        playMPExitRender(screen.renderer, &playMPExit, &assets, &board, &game);
      }
      break;

    case SCREEN_SETTINGS:
      settingsScreenUpdate(&settingsScreen, &input, &currentScreen, &settings,
                           previousScreen);
      settingsScreenRender(screen.renderer, &settingsScreen, &assets);
      break;

    case SCREEN_EXIT:
      // printf("DEBUG main: Cambiando a SCREEN_EXIT\n");
      running = false;
      break;

    default:
      printf("DEBUG main: Screen desconocida: %d\n", currentScreen);
      break;
    }

    if (currentScreen == SCREEN_SET_CARDS &&
        (lastScreenState == SCREEN_CONFIG_SINGLE ||
         lastScreenState == SCREEN_CONFIG_MULTI)) {
      configSource = lastScreenState;
      // printf("DEBUG: Guardado configSource ID: %d\n", configSource);
    }

    SDL_RenderPresent(screen.renderer);

    SDL_Delay(16);
  }

  sound_finish();
  playSPExitDestroy(&playSPExit);
  playMPExitDestroy(&playMPExit);
  playMPDestroy(&playMP, &game);
  playSPDestroy(&playSP);
  settingsScreenDestroy(&settingsScreen);
  setDiffMenuDestroy(&setDiff);
  setCardMenuDestroy(&setCard);
  multiPlayerDestroy(&multiPlayer);
  singlePlayerDestroy(&singlePlayer);
  mainMenuDestroy(&mainMenu);
  boardDestroy(&board);
  assetsUnload(&assets);
  screenShutdown(&screen);
  printf("Pantalla cerrada.\n¡Hasta luego!");

  return 0;
}
