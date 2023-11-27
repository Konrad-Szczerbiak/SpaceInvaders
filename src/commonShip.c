//
// Created by szczerbiakko on 04.11.23.
//

#include "utilities.h"
#include "SDL2/SDL_image.h"
#include "commonShip.h"
#include "draw.h"

#define DEFAULT_SHIP_SPEED 1

typedef enum {
    eGfxPlayer,
    eGfxEnemy,
    eGfxLaser,
    eGfxTypesCnt,
} E_GfxTypes;

static T_ShipsList mg_shipList[eShipTypesCnt] = {0};

typedef struct {
    SDL_Surface* pGfxSurface;
    SDL_Texture* pGfxTexture;
} T_GfhHandle;

static T_GfhHandle mg_Graphics[eGfxTypesCnt] = {0};

/*currently used display representation as a rectangle*/
static SDL_Rect mg_windowDimension = {0};

/*STATIC FUNCS*/
#define LASER_WIDTH_PXLS 5
#define LASER_HEIGHT_PXLS 15

static void alignLaserWithShip(T_Ship* pShip);
static void createLasersForShip(T_Ship* pShip);
static T_Ship* createShip(E_ShipType type, int xPos, int yPos);
static void deleteShip(E_ShipType type, T_Ship* pShip);
static T_GfhHandle getGfxStruct(SDL_Renderer* pRenderer, E_GfxTypes gfxType);
/*STATIC FUNCS*/

const T_Ship* ShipList_GetElementShipPtrByIndex(const T_ShipsList * const pList, int index)
{
    if (index >= pList->count)
    {
        assert(0);
        return NULL;
    }
    const bool startFromTail = (2*index > pList->count);
    startFromTail && (index = (pList->count-1) - index);
    T_ShipsListElement* pRet = startFromTail ? pList->tail : pList->head;
    while (index--)
    {
        pRet = startFromTail ? pRet->pPrev : pRet->pNext;
    }
    return pRet->ship;
}

int ShipList_GetElementIndexByShipPtr(const T_ShipsList * const pList, const T_Ship* const pShip)
{
    int index = 0;
    for (T_ShipsListElement* elem = pList->head; elem != NULL; elem = elem->pNext)
    {
        if (pShip == elem->ship) break;
        index++;
    }
    return index;
}

void ShipsList_PushBack(T_ShipsList* pList, T_Ship* pNewElem)
{
    bool isEmpty = pList->count == 0;

    T_ShipsListElement* new = calloc(1, sizeof(T_ShipsListElement));
    new->ship = pNewElem;
    new->pPrev = (!isEmpty) ? pList->tail : NULL;
    if (!isEmpty)
    {
        pList->tail->pNext = new;
    }
    pList->tail = (!isEmpty) ? new : (pList->head = new);

    pList->count++;
}

void ShipsList_PopBack(T_ShipsList* pList)
{
    if (pList->count == 0)
    {
        return;
    }

    T_ShipsListElement* pTmp = pList->tail->pPrev;
    memset(pList->tail->ship, 0, sizeof(T_Ship));
    free(pList->tail->ship);
    memset(pList->tail, 0, sizeof(T_ShipsListElement));
    free(pList->tail);
    pList->tail = pTmp;
    (pList->tail != NULL) ? (pList->tail->pNext = NULL) : (pList->head = NULL);
    pList->count--;
}

void ShipList_PopByIndex(T_ShipsList* pList, int elementIndex)
{
    if (pList->count == 0 || pList->count <= elementIndex)
    {
        return;
    }
    else if (pList->count == elementIndex + 1)
    {
        ShipsList_PopBack(pList);
        return;
    }

    T_ShipsListElement* pReqElement = pList->head;
    while(elementIndex--)
    {
        pReqElement = pReqElement->pNext;
    }
    (pList->head != pReqElement) ? (pReqElement->pPrev->pNext = pReqElement->pNext) : (pList->head = pReqElement->pNext);
    pReqElement->pNext->pPrev = pReqElement->pPrev;
    memset(pReqElement->ship, 0, sizeof(T_Ship));
    free(pReqElement->ship);
    memset(pReqElement, 0, sizeof(T_ShipsListElement));
    free(pReqElement);
    pList->count--;
}

static void assignGraphics(T_GfhHandle* paGfx, SDL_Renderer* pRenderer)
{
    for (E_GfxTypes gfxType = eGfxPlayer; gfxType < eGfxTypesCnt; gfxType++)
    {
        paGfx[gfxType] = getGfxStruct(pRenderer, gfxType);
    }
}

void CommonShip_InitModule(SDL_Renderer* pRenderer, void* pScreenInfo, SDL_Rect* pWindowBorders)
{
    assignGraphics(mg_Graphics, pRenderer);
    memcpy(&mg_windowDimension, pWindowBorders, sizeof(SDL_Rect));
}

static T_GfhHandle getGfxStruct(SDL_Renderer* pRenderer, E_GfxTypes gfxType)
{
    T_GfhHandle ret;

    switch (gfxType)
    {
        case eGfxPlayer:
            ret.pGfxSurface = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/DETAILED_PLAYER.png");
            break;
        case eGfxEnemy:
            ret.pGfxSurface = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/ENEMY.png");
            break;
        case eGfxLaser:
            ret.pGfxSurface = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/LASER_BOLT.png");
            break;
        default:
            assert(0);
            exit(-1);
    }
    if (NULL == ret.pGfxSurface)
    {
        assert(0);
        exit(-1);
    }

    ret.pGfxTexture = SDL_CreateTextureFromSurface(pRenderer, ret.pGfxSurface);
    if (NULL == ret.pGfxTexture)
    {
        assert(0);
        exit(-1);
    }

    return ret;
}

static void alignLaserWithShip(T_Ship* pShip)
{
    pShip->lasers.lasLeft.x = pShip->shipHitbox.x;
    pShip->lasers.lasRight.x = pShip->shipHitbox.x + pShip->shipHitbox.w - pShip->lasers.lasRight.w;
    pShip->lasers.lasLeft.y = pShip->lasers.lasRight.y = pShip->shipHitbox.y;
}

static void createLasersForShip(T_Ship* pShip)
{
    pShip->lasers.lasLeft.w = pShip->lasers.lasRight.w = LASER_WIDTH_PXLS;
    pShip->lasers.lasLeft.h = pShip->lasers.lasRight.h = LASER_HEIGHT_PXLS;
    pShip->lasers.lasTexture = mg_Graphics[eGfxLaser].pGfxTexture;
    alignLaserWithShip(pShip);
}

T_Ship* CommonShip_CreateShip(E_ShipType type, int xPos, int yPos)
{
    T_Ship* new = calloc(1, sizeof(T_Ship));

    new->shipHitbox.h = mg_windowDimension.h / 10;
    new->shipHitbox.w = mg_windowDimension.w / 20;
    new->shipHitbox.x = xPos;
    new->shipHitbox.y = yPos;

    new->mvmntBorder.x = mg_windowDimension.x;
    new->mvmntBorder.y = mg_windowDimension.y;
    new->mvmntBorder.w = mg_windowDimension.w - new->shipHitbox.w;
    new->mvmntBorder.h = mg_windowDimension.h - new->shipHitbox.h;
    new->shipTexture = mg_Graphics[eEnemyShip == type ? eGfxEnemy : eGfxPlayer].pGfxTexture;

    new->renderAngle = eEnemyShip == type ? 180.F : .0F;
    new->borderDirReached = 0xff;

    new->speed = DEFAULT_SHIP_SPEED;

    createLasersForShip(new);

    ShipsList_PushBack(&mg_shipList[type], new);
}

void CommonShip_DeleteShip(E_ShipType type, T_Ship* pShip)
{
    int index = ShipList_GetElementIndexByShipPtr(&mg_shipList[type], pShip);
    ShipList_PopByIndex(&mg_shipList[type], index);
}

static void manageShipLaserShots(T_Ship* pShip)
{
    pShip->lasers.lasLeft.y += 1;
    pShip->lasers.lasRight.y += 1;
    if(CommonShip_isMvmntBorderReached(&pShip->lasers.lasLeft, DownBorder) || CommonShip_isMvmntBorderReached(&pShip->lasers.lasRight, DownBorder) )
    {
        pShip->isShooting = false;
        alignLaserWithShip(pShip);
    }
}

void CommonShip_RenderShip(T_Ship* pShip)
{
    SDL_Renderer* pRenderer = Draw_GetRendererPtr();
    if (false == pShip->isHit)
    {
        SDL_RenderCopyEx(pRenderer, pShip->shipTexture, NULL,  &pShip->shipHitbox, pShip->renderAngle, NULL, SDL_FLIP_NONE);
        if (pShip->isShooting)
        {
            SDL_RenderCopyEx(pRenderer, pShip->lasers.lasTexture, NULL,  &pShip->lasers.lasRight, pShip->renderAngle, NULL, SDL_FLIP_NONE);
            SDL_RenderCopyEx(pRenderer, pShip->lasers.lasTexture, NULL,  &pShip->lasers.lasLeft, pShip->renderAngle, NULL, SDL_FLIP_NONE);
            manageShipLaserShots(pShip);
        }
        else
        {
            alignLaserWithShip(pShip);
        }
    }
    else
    {
        alignLaserWithShip(pShip);
//        if (++enemyHitDissapearCyclesCnt >= 2000)
//        {
//            isEnemyHit = false;
//            enemyHitDissapearCyclesCnt = 0;
//        }
    }
}

void ShipList_PerformForEach(T_ShipsList* pList, void(*pCb)(T_Ship*))
{
    if (0 == pList->count)
    {
        return;
    }

    for (T_ShipsListElement* elem = pList->head; elem != NULL; elem = elem->pNext)
    {
        pCb(elem->ship);
    }
}

T_ShipsList* CommonShip_GetShipListPtr(E_ShipType type)
{
    return &mg_shipList[type];
}

void CommonShip_SetShipShootingFlag(T_Ship* pShip)
{
    pShip->isShooting = true;
}

void CommonShip_ClearShipShootingFlag(T_Ship* pShip)
{
    pShip->isShooting = false;
}

bool CommonShip_isMvmntBorderReached(SDL_Rect* pHitbox, E_Border borderType)
{
    switch (borderType) {
        case UpBorder:
            return (pHitbox->y <= mg_windowDimension.y);
        case DownBorder:
            return (pHitbox->y >= mg_windowDimension.y + mg_windowDimension.h);
        case LeftBorder:
            return (pHitbox->x <= mg_windowDimension.x);
        case RightBorder:
            return (pHitbox->x >= mg_windowDimension.x + mg_windowDimension.w);
        default:
            return false;
    }
}

void CommonShip_Move(E_MovementDirection dir, T_Ship* pShip)
{
    int* actCoordinate = NULL;
    int step = 0;
    switch (dir)
    {
        case eDirectionRight: actCoordinate = &pShip->shipHitbox.x; step = (+1); break;
        case eDirectionLeft: actCoordinate = &pShip->shipHitbox.x; step = (-1); break;
        case eDirectionDown: actCoordinate = &pShip->shipHitbox.y; step = (+1); break;
        case eDirectionUp: actCoordinate = &pShip->shipHitbox.y; step = (-1); break;
        default:
            assert(0);
            return;
    }
    *actCoordinate+=(pShip->speed)*step;
}

void CommonShip_MovePlayer(E_MovementDirection dir, int playerNr)
{
    T_Ship* pPlayerShip = ShipList_GetElementShipPtrByIndex(&mg_shipList[ePlayerShip], --playerNr);
    CommonShip_Move(dir, pPlayerShip);
}


/*okej - jak zarejestrować kolizję pomiędzy dwoma elementami? A dokładniej - pomiędzy laserami gracza a którymś ze statków?*/
/*mógłbym, na przykład: sprawdzać każdy element listy w osobnym watku, uruchamianym semaforem gdy gracz wystrzeli.*/

