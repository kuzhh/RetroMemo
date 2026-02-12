#include "input.h"

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
    s�lo modifica estados:
        devuelve TRUE si contin�a la ejecuci�n del programa
        devuelve FALSE si la ejecuci�n del programa se corta
    NO interpreta
    NO dibuja
    NO renderea
    */

    SDL_Event event;

    input->mousePressed = 0;
    input->mouseReleased = 0;

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
