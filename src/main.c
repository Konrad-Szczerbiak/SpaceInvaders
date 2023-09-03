#include <stdio.h>
#include "draw.h"
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "utilities.h"

int main()
{
    Utilities_ConfigureErrorHandling();

    SDL_Init(SDL_INIT_EVERYTHING);

    Draw_ModuleInit();

    while (1)
    {
        sleep(1);
    }

}
