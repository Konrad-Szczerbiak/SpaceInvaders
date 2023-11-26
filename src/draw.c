#include "utilities.h"
#include "SDL2/SDL_image.h"
#include "draw.h"
#include "enemyAI.h"
#include "commonShip.h"

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

SDL_Rect mg_PlayerMovementBorder = {0};

/*To powinno być przeniesione do innego modułu*/
static SDL_Rect mg_playerHitbox = {0};

_Noreturn static T_ThreadFunc Draw_ThreadFunction(void* argv);

static bool isPlayerHit = false;

pthread_mutex_t shootingInfoMutx = {0};

E_OpResult Draw_ModuleInit(void)
{
    pthread_mutex_init(&shootingInfoMutx, NULL);
    pthread_create(&mg_drawThread, &mg_drawThreadAttr, &Draw_ThreadFunction, NULL);
}

static bool isShooting = false;
//static bool isEnemyShooting = false;

void setShootingtrue(void)
{
    pthread_mutex_lock(&shootingInfoMutx);
    isShooting = true;
    pthread_mutex_unlock(&shootingInfoMutx);
}

_Noreturn static T_ThreadFunc Draw_ThreadFunction(void* argv)
{
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

    EnemyAI_Init();

    CommonShip_InitModule(mg_pRenderer, NULL, &mg_windowPosition);

    while (1)
    {
        SDL_RenderPresent(mg_pRenderer);
        ThreadSleep(1);
        SDL_RenderClear(mg_pRenderer);

        ShipList_PerformForEach(CommonShip_GetShipListPtr(eEnemyShip), CommonShip_RenderShip);
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

#define PLAYER_SHIP_SPEED 10

int MovePlayerLeft(void)
{
    if (Draw_isMvmntBorderReached(LeftBorder, mg_playerHitbox.x-=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.x = mg_PlayerMovementBorder.x;
    }
}

int MovePlayerRight(void)
{
    if (Draw_isMvmntBorderReached(RightBorder, mg_playerHitbox.x+=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.x = mg_PlayerMovementBorder.x + mg_PlayerMovementBorder.w;
    }
}

int MovePlayerUp(void)
{
    if (Draw_isMvmntBorderReached(UpBorder, mg_playerHitbox.y-=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.y = mg_PlayerMovementBorder.y;
    }
}

int MovePlayerDown(void)
{
    if (Draw_isMvmntBorderReached(DownBorder, mg_playerHitbox.y+=PLAYER_SHIP_SPEED))
    {
        mg_playerHitbox.y = mg_PlayerMovementBorder.y + mg_PlayerMovementBorder.h;
    }
}

bool Draw_isMvmntBorderReached(E_Border borderType, int positionValue)
{
    switch (borderType) {
        case UpBorder:
            return (positionValue <= mg_windowPosition.y);
        case DownBorder:
            return (positionValue >= mg_windowPosition.y + mg_windowPosition.h);
        case LeftBorder:
            return (positionValue <= mg_windowPosition.x);
        case RightBorder:
            return (positionValue >= mg_windowPosition.x + mg_windowPosition.w);
        default:
            return false;
    }
}

SDL_Renderer* Draw_GetRendererPtr(void)
{
    return mg_pRenderer;
}
