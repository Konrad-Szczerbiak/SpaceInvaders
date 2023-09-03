#include "draw.h"
#include "utilities.h"
#include "inputs.h"

int main()
{
    Utilities_ConfigureErrorHandling();

    SDL_Init(SDL_INIT_EVERYTHING);

    Draw_ModuleInit();

    Inputs_ModuleInit();

    while (1)
    {
        ThreadSleep(1);
    }

}
