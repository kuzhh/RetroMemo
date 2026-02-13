#include "input.h"
#include <string.h>

int pointInRect(int x, int y, SDL_Rect* r)
{
    return (x >= r->x &&
            x <= r->x + r->w &&
            y >= r->y &&
            y <= r->y + r->h);
}

bool handleEvents(tInput* input)
{
    /*
    solo modifica estados:
        devuelve TRUE si continua la ejecucion del programa
        devuelve FALSE si la ejecucion del programa se corta
    NO interpreta
    NO dibuja
    NO renderea
    */

    SDL_Event event;

    input->mousePressed = 0;
    input->mouseReleased = 0;
    memset(input->textInput, 0, MAX_TEXT_INPUT);

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_MOUSEMOTION:
            input->mouseX = event.motion.x;
            input->mouseY = event.motion.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                input->mouseDown = 1;
                input->mousePressed = 1;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                input->mouseDown = 0;
                input->mouseReleased = 1;
            }
            break;

        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                return false;
            if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                if(input->textInputLen > 0)
                    input->textInputLen--;
            }
            break;

        case SDL_TEXTINPUT:
            if(input->textInputLen < MAX_TEXT_INPUT - 1)
            {
                for(int i = 0; event.text.text[i] && input->textInputLen < MAX_TEXT_INPUT - 1; i++)
                {
                    input->textInput[input->textInputLen++] = event.text.text[i];
                }
            }
            break;

            /* para dps :)
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    //click izq
                }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    //click der
                }
            */
            default:
                break;
        }
    }

    return true;
}
