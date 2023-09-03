#include "utilities.h"
#include "SDL2/SDL_image.h"

/*
 * This module should handle rendering etc, so first of all - we have to move everything related to rendering here
 */

/*
 * Chce zeby byl to modul odpowiedzialny za renderowanie, wiec renderer i window idzie  tutaj - tak samo jak task renderowania
 * */

#define DEF_SCREEN_COLOR_HEX 0x00252b52

static pthread_t mg_drawThread = 0;
static pthread_attr_t mg_drawThreadAttr = {0};

static SDL_DisplayMode mg_screenDisplayMode = {0};
static SDL_Window* mg_pWindow = NULL;
static SDL_DisplayMode mg_windowDisplayMode = {0};
static SDL_Rect mg_windowPosition = {0};
static SDL_Renderer* mg_pRenderer = NULL;
static T_HexColor screenColor = {.alphahex = DEF_SCREEN_COLOR_HEX};

static SDL_Texture* mg_playerTexture = NULL;

/*To powinno być przeniesione do innego modułu*/
static SDL_Rect mg_playerHitbox = {0};

_Noreturn static T_ThreadFunc Draw_ThreadFunction(void* argv);
static SDL_Rect createPlayer(void);

E_OpResult Draw_ModuleInit(void)
{
        pthread_create(&mg_drawThread, &mg_drawThreadAttr, &Draw_ThreadFunction, NULL);
}

_Noreturn static T_ThreadFunc Draw_ThreadFunction(void* argv)
{
    /*Get display mode*/
    if (0 != SDL_GetDesktopDisplayMode(0, &mg_screenDisplayMode))
    {
        assert(0);
        exit(-1);
    }

    /*Create window - turn into func*/
    mg_pWindow = SDL_CreateWindow(
            /*title*/"Game Window",
            /*x*/SDL_WINDOWPOS_CENTERED,
            /*y*/SDL_WINDOWPOS_CENTERED,
            /*w*/mg_screenDisplayMode.w,
            /*h*/mg_screenDisplayMode.h,
            /*flags*/SDL_WINDOW_FULLSCREEN
            );
    if (NULL == mg_pWindow)
    {
        assert(0);
        exit(-1);
    }
    SDL_GetWindowDisplayMode(mg_pWindow, &mg_windowDisplayMode);
    SDL_GetWindowPosition(mg_pWindow, &mg_windowPosition.x, &mg_windowPosition.y);
    mg_windowPosition.w = mg_windowDisplayMode.w;
    mg_windowPosition.h = mg_windowDisplayMode.h;

    /*Create renderer - turn into func*/
    mg_pRenderer = SDL_CreateRenderer(
            /*window*/mg_pWindow,
            /*index*/-1,
            /*flags*/SDL_RENDERER_ACCELERATED
            );
    if (NULL == mg_pRenderer)
    {
        assert(0);
        exit(-1);
    }

    /*Set color to flush with*/
    if (0 != SDL_SetRenderDrawColor(mg_pRenderer, screenColor.red, screenColor.green, screenColor.blue, screenColor.alpha))
    {
        assert(0);
        exit(-1);
    }

    if (0 != SDL_RenderClear(mg_pRenderer))
    {
        assert(0);
        exit(-1);
    }

    mg_playerHitbox = createPlayer();


    while (1)
    {
        SDL_RenderPresent(mg_pRenderer);
        ThreadSleep(1);
        SDL_RenderClear(mg_pRenderer);
        SDL_RenderCopy(mg_pRenderer, mg_playerTexture, NULL, &mg_playerHitbox);
    }

}


/*jak mamy rejestrować poszczególne tekstury?
 * może robimy listę?
 * albo na razie dynamiczna alokacja pamięci?
 * spróbujmy dynamiczną alokację... na początek...
 * */

typedef struct {
    SDL_Texture* playerTexture;
    SDL_Rect playerHitbox;
} T_Player;

static T_Player mg_players[1] = {0};

static SDL_Rect createPlayer(void)
{

    SDL_Rect playerEntity = {
            .h = mg_windowDisplayMode.h / 10,
            .w = mg_windowDisplayMode.w / 20,
            .x = mg_windowPosition.w / 2,
            .y = mg_windowPosition.h / 2,
    };

    SDL_Surface* playerTextureImage = NULL;
    playerTextureImage = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/SUPER_SMPL_PLAYER.png");
    if (NULL == playerTextureImage)
    {
        assert(0);
        exit(-1);
    }

    mg_playerTexture = SDL_CreateTextureFromSurface(mg_pRenderer, playerTextureImage);
    if (NULL == mg_playerTexture)
    {
        assert(0);
        exit(-1);
    }

    return playerEntity;
}

#define PLAYER_SHIP_SPEED 10

int MovePlayerLeft(void)
{
    mg_playerHitbox.x-=PLAYER_SHIP_SPEED;
}

int MovePlayerRight(void)
{
    mg_playerHitbox.x+=PLAYER_SHIP_SPEED;
}

int MovePlayerUp(void)
{
    mg_playerHitbox.y-=PLAYER_SHIP_SPEED;
}

int MovePlayerDown(void)
{
    mg_playerHitbox.y+=PLAYER_SHIP_SPEED;
}