#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "library.h"

typedef struct
{
    int mouseX;
    int mouseY;
    int mouseDown; //presionado ahora
    int mousePressed; //se presionó
    int mouseReleased; //se soltó
    char textInput[MAX_TEXT_INPUT]; //buffer para input de texto
    int textInputLen; //longitud del texto ingresado
    int textActive;
} tInput;

bool handleEvents(tInput*);
int pointInRect(int, int, SDL_Rect*);

#endif // INPUT_H_INCLUDED
