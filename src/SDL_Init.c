#include "SDL_Init.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SDL_FLAGS_NONE 0x00

static const char* _SCG_windowTitle = "TITEL";

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window * window;
} T_BasicApp;

static T_BasicApp _SG_Application = {0};

E_OpResult SDL2_Init(void)
{
   const  char* SDL_Err = NULL;
    /// renderer flags and windows flags:
    struct flags
    {
        int renderer;
        int window;
        int libIMG;
    } Flags;

    Flags.renderer = SDL_RENDERER_ACCELERATED;
    Flags.window = SDL_FLAGS_NONE;
    Flags.libIMG = IMG_INIT_JPG | IMG_INIT_PNG;


    if (SDL_Init(SDL_INIT_VIDEO) == eSuccess)
    {
        if ((_SG_Application.window = SDL_CreateWindow
                                                                (
                                                                        _SCG_windowTitle,
                                                                        SDL_WINDOWPOS_CENTERED,
                                                                        SDL_WINDOWPOS_CENTERED,
                                                                        WINDOW_WIDTH,
                                                                        WINDOW_HEIGHT,
                                                                        0
                                                                 )
        ) != NULL)
        {
            if ((_SG_Application.renderer = SDL_CreateRenderer
                                                                    (
                                                                        _SG_Application.window,
                                                                        -1,
                                                                        0
                                                                    )
            ) != NULL)
            {
                if (Flags.libIMG & IMG_Init(Flags.libIMG))
                {
                    return eSuccess;
                }
                else
                {
                    SDL_Err = SDL_GetError();
                    printf("\n%s\n", SDL_Err);
                }
            }
            else
            {
                SDL_Err = SDL_GetError();
                printf("\n%s\n", SDL_Err);
            }
        }
        else
        {
            SDL_Err = SDL_GetError();
            printf("\n%s\n", SDL_Err);
        }

    }
    else
    {
        SDL_Err = SDL_GetError();
        printf("\n%s\n", SDL_Err);
    }
}

void* getRendererPtr(void)
{
    return (void*)_SG_Application.renderer;
}

void GameQuit(void)
{
    SDL_Quit();
    exit(0);
}