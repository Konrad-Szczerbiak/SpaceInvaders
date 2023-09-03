#include "utilities.h"
#include "stdlib.h"
#include "SDL2/SDL.h"
#include "pthread.h"

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