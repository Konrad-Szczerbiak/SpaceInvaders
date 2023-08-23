#include "utilities.h"
#include "stdlib.h"
#include "SDL2/SDL.h"

static void displayErrorMsg(void);

void Utilities_ConfigureErrorHandling(void)
{
    atexit(displayErrorMsg);
}

static void displayErrorMsg(void)
{
    const char* errMsg = SDL_GetError();
    printf("Failure. Last error message was: %s\n", errMsg);
}

inline void sleep(int ms)
{
    SDL_Delay(ms);
}
