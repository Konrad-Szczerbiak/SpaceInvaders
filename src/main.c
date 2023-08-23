#include <stdio.h>
#include "draw.h"
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "utilities.h"
#define ERR_CANNOT_LOAD_TEXTURE 69

#define SDL_ASSERT_LEVEL 3 /*Super detailed debug*/
//#include <SDL_assert.h>

void performAtExit(void)
{
    printf("COULD NOT LOAD TEXTURE FILE - ERRCODE: %d", ERR_CANNOT_LOAD_TEXTURE);
}

int main()
{
    Utilities_ConfigureErrorHandling();

    /* Iwant to be able to:
     *
     * 1. generate a game field
     *      let's start with that shall we? :)
     */
    /* we need to initialise SDL*/
    SDL_Init(SDL_INIT_EVERYTHING);

    //    SDL_Surface* pPlayerGfx = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/SUPER_SMPL_PLAYER.png");
//
//    SDL_Texture* pPlayerTexture = NULL;
//    pPlayerTexture = SDL_CreateTextureFromSurface(pGameScreenRenderer, pPlayerGfx);
//
//    SDL_Rect playerRect;
//    playerRect.h = 50;
//    playerRect.w = 50;
//    playerRect.x = 0;
//    playerRect.y = 0;
//
//    SDL_RenderCopy(pGameScreenRenderer, pPlayerTexture, NULL, NULL);
    Draw_ModuleInit();

    while (1)
    {
        sleep(1);
    }

    SDL_Quit();/*Deinitialise*/

    return 0;
}
