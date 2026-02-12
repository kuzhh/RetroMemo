#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdbool.h>

#define OK 0
#define SDL_ERR 1

typedef struct
{
    int mouseX;
    int mouseY;
    int mouseDown; //presionado ahora
    int mousePressed; //se presionó
    int mouseReleased; //se soltó
} tInput;

bool handleEvents(tInput*);
int pointInRect(int, int, SDL_Rect*);

#endif // INPUT_H_INCLUDED
