#include "utilities.h"
#include "SDL2/SDL_image.h"
#include "draw.h"
#include "enemyAI.h"
/*
 * This module should handle rendering etc, so first of all - we have to move everything related to rendering here
 */

/*
 * Chce zeby byl to modul odpowiedzialny za renderowanie, wiec renderer i window idzie  tutaj - tak samo jak task renderowania
 * */

#define DEF_SCREEN_COLOR_HEX 0x00252b52

typedef enum {
    UpBorder,
    LeftBorder,
    DownBorder,
    RightBorder,
} E_Border;

static pthread_t mg_drawThread = 0;
static pthread_attr_t mg_drawThreadAttr = {0};

static SDL_DisplayMode mg_screenDisplayMode = {0};
static SDL_Window* mg_pWindow = NULL;
static SDL_DisplayMode mg_windowDisplayMode = {0};
static SDL_Rect mg_windowPosition = {0};
static SDL_Renderer* mg_pRenderer = NULL;
static T_HexColor screenColor = {.alphahex = DEF_SCREEN_COLOR_HEX};

static SDL_Texture* mg_playerTexture = NULL;
static SDL_Texture* mg_enemyTexture = NULL;

SDL_Rect mg_PlayerMovementBorder = {0};

SDL_Rect mg_EnemyMovementBorder = {0};

/*To powinno być przeniesione do innego modułu*/
static SDL_Rect mg_playerHitbox = {0};

static SDL_Rect mg_enemyHitbox = {0};

static T_Laser mg_playerLasers = {0};

static T_Ship mg_players[1] = {0};
static T_Ship mg_enemy[1] = {0};

_Noreturn static T_ThreadFunc Draw_ThreadFunction(void* argv);
static void createPlayerLasers(void);
static void createPlayer(void);
void alignlasersWithPlayer(void);

static bool isBorderReached(E_Border borderType, int positionValue);

E_OpResult Draw_ModuleInit(void)
{
        pthread_create(&mg_drawThread, &mg_drawThreadAttr, &Draw_ThreadFunction, NULL);
}

static bool isShooting = false;

void setShootingtrue(void)
{
    isShooting = true;
}

static void manageLaserShots(void)
{
    mg_playerLasers.lasLeft.y -= 1;
    mg_playerLasers.lasRight.y -= 1;
    if(isBorderReached(UpBorder, mg_playerLasers.lasLeft.y) || isBorderReached(UpBorder, mg_playerLasers.lasRight.y) )
    {
        isShooting = false;
        alignlasersWithPlayer();
    }
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

    createPlayer();
    createPlayerLasers();
    EnemyAI_Init();

    while (1)
    {
        SDL_RenderPresent(mg_pRenderer);
        ThreadSleep(1);
        SDL_RenderClear(mg_pRenderer);
        SDL_RenderCopy(mg_pRenderer, mg_playerTexture, NULL, &mg_playerHitbox);
        SDL_RenderCopyEx(mg_pRenderer, mg_enemy[0].playerTexture, NULL,  &mg_enemy[0].playerHitbox, 180, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(mg_pRenderer, mg_playerLasers.lasTexture, NULL,  &mg_playerLasers.lasRight);
        SDL_RenderCopy(mg_pRenderer, mg_playerLasers.lasTexture, NULL,  &mg_playerLasers.lasLeft);
        if (isShooting)
        {
            manageLaserShots();
        }
        else
        {
            alignlasersWithPlayer();
        }
    }

}

void alignlasersWithPlayer(void)
{
    mg_playerLasers.lasLeft.x = mg_playerHitbox.x;
    mg_playerLasers.lasRight.x = mg_playerHitbox.x + mg_playerHitbox.w - mg_playerLasers.lasRight.w;
    mg_playerLasers.lasLeft.y = mg_playerLasers.lasRight.y = mg_playerHitbox.y;
}

static void createPlayerLasers(void)
{
    mg_playerLasers.lasLeft.w = mg_playerLasers.lasRight.w = 5;
    mg_playerLasers.lasLeft.h = mg_playerLasers.lasRight.h = 15;
    alignlasersWithPlayer();

    SDL_Surface* laserTextureSrfc = NULL;
    laserTextureSrfc = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/LASER_BOLT.png");
    if (NULL == laserTextureSrfc)
    {
        assert(0);
        exit(-1);
    }

    mg_playerLasers.lasTexture = SDL_CreateTextureFromSurface(mg_pRenderer, laserTextureSrfc);
    if (NULL == mg_playerLasers.lasTexture)
    {
        assert(0);
        exit(-1);
    }
}

static void createPlayer(void)
{
    mg_playerHitbox.h = mg_windowDisplayMode.h / 10;
    mg_playerHitbox.w = mg_windowDisplayMode.w / 20;
    mg_playerHitbox.x = mg_windowPosition.w / 2;
    mg_playerHitbox.y = mg_windowPosition.h / 2;

    mg_PlayerMovementBorder.x = mg_windowPosition.x;
    mg_PlayerMovementBorder.y = mg_windowPosition.y;
    mg_PlayerMovementBorder.w = mg_windowPosition.w - mg_playerHitbox.w;
    mg_PlayerMovementBorder.h = mg_windowPosition.h - mg_playerHitbox.h;

    SDL_Surface* playerTextureImage = NULL;
    playerTextureImage = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/DETAILED_PLAYER.png");
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
}

void Draw_CreateEnemy(int x, int y)
{
    mg_enemy[0].playerHitbox.h = mg_windowDisplayMode.h / 10;
    mg_enemy[0].playerHitbox.w = mg_windowDisplayMode.w / 20;
    mg_enemy[0].playerHitbox.x = x;
    mg_enemy[0].playerHitbox.y = y;

    mg_enemy[0].mvmntBorder.x = mg_windowPosition.x;
    mg_enemy[0].mvmntBorder.y = mg_windowPosition.y;
    mg_enemy[0].mvmntBorder.w = mg_windowPosition.w - mg_enemy[0].playerHitbox.w;
    mg_enemy[0].mvmntBorder.h = mg_windowPosition.h - mg_enemy[0].playerHitbox.h;

    SDL_Surface* enemyTextureSurface = NULL;
    enemyTextureSurface = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/ENEMY.png");
    if (NULL == enemyTextureSurface)
    {
        assert(0);
        exit(-1);
    }

    while (!mg_pRenderer)
    {
        ThreadSleep(1);
    }

    mg_enemy[0].playerTexture = SDL_CreateTextureFromSurface(mg_pRenderer, enemyTextureSurface);
    if (NULL == mg_enemy[0].playerTexture)
    {
        assert(0);
        exit(-1);
    }
}

 T_Ship* getEnemyPtr(void)
{
    return &mg_enemy[0];
}

#define PLAYER_SHIP_SPEED 10

int MovePlayerLeft(void)
{
    if (isBorderReached(LeftBorder, mg_playerHitbox.x-=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.x = mg_PlayerMovementBorder.x;
    }
}

int MovePlayerRight(void)
{
    if (isBorderReached(RightBorder, mg_playerHitbox.x+=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.x = mg_PlayerMovementBorder.x + mg_PlayerMovementBorder.w;
    }
}

int MovePlayerUp(void)
{
    if (isBorderReached(UpBorder, mg_playerHitbox.y-=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.y = mg_PlayerMovementBorder.y;
    }
}

int MovePlayerDown(void)
{
    if (isBorderReached(DownBorder, mg_playerHitbox.y+=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.y = mg_PlayerMovementBorder.y + mg_PlayerMovementBorder.h;
    }
}

static bool isBorderReached(E_Border borderType, int positionValue)
{
    switch (borderType) {
        case UpBorder:
            return (positionValue <= mg_PlayerMovementBorder.y);
        case DownBorder:
            return (positionValue >= mg_PlayerMovementBorder.y + mg_PlayerMovementBorder.h);
        case LeftBorder:
            return (positionValue <= mg_PlayerMovementBorder.x);
        case RightBorder:
            return (positionValue >= mg_PlayerMovementBorder.x + mg_PlayerMovementBorder.w);
        default:
            return false;
    }
}

