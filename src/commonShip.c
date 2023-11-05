//
// Created by szczerbiakko on 04.11.23.
//

#include "utilities.h"
#include "SDL2/SDL_image.h"
#include "commonShip.h"

typedef struct {
    SDL_Texture* lasTexture;
    SDL_Rect lasLeft;
    SDL_Rect lasRight;
} T_Laser;

typedef struct {
    SDL_Rect mvmntBorder;
    SDL_Texture* playerTexture;
    SDL_Rect shipHitbox;
    T_Laser lasers;
} T_Ship;

typedef struct S_ShipsListElement{
    T_Ship playerShip;
    struct S_ShipsListElement* pNext;
    struct S_ShipsListElement* pPrev;
} T_ShipsListElement;

typedef struct {
    T_ShipsListElement* head;
    T_ShipsListElement* tail;
    int count;
} T_ShipsList;

static T_ShipsList mg_Enemies = {0};

typedef struct {
    SDL_Surface* pGfxSurface;
    SDL_Texture* pGfxTexture;
} T_GfhHandle;

static T_GfhHandle mg_TexturesMap[eShipTypesCnt] = {0};

/*currently used display representation as a rectangle*/
static SDL_Rect mg_windowDimension = {0};

/*STATIC FUNCS*/
static T_GfhHandle getGfxStruct(SDL_Renderer* pRenderer, E_ShipType type);
static T_Ship* createShip(E_ShipType type, int xPos, int yPos);

void ShipsList_PushBack(T_ShipsList* pList, T_Ship* pNewElem)
{
    bool isEmpty = pList->count == 0;

    T_ShipsListElement* new = calloc(1, sizeof(T_ShipsListElement));
    memcpy(&new->playerShip, pNewElem, sizeof(T_Ship));
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
    memset(pList->tail, 0, sizeof(T_ShipsListElement));
    free(pList->tail);
    pList->tail = pTmp;
    (pList->tail != NULL) ? (pList->tail->pNext = NULL) : (pList->head = NULL);
    pList->count--;
}

T_Ship* commonShip_CreateShip(E_ShipType shipType)
{
    /*Zaczniemy od generowania statków w lewym górnym rogu ekranu*/
    T_Ship* new = calloc(1, sizeof(T_Ship));



}


struct S_ShipInterface
{

};



/*create interface to manage the ship from within this module*/
/*ok zastanów się, nie trzeba od razu
 * Można na przykład zacząc od określenia: czym ma byc ten modułi co ma zawierać?
 *
 * Ma zawierać - ogólną strukturę statku - jest już
 * informacje na temat statków tylko i wyłącznie
 * Interfejs do poruszania statkami*/
void CommonShip_InitModule(SDL_Renderer* pRenderer, void* pScreenInfo, SDL_Rect* pWindowBorders);
void CommonShip_InitModule(SDL_Renderer* pRenderer, void* pScreenInfo, SDL_Rect* pWindowBorders)
{
    mg_TexturesMap[ePlayerShip] = getGfxStruct(pRenderer, ePlayerShip);
    mg_TexturesMap[eEnemyShip] = getGfxStruct(pRenderer, eEnemyShip);

    memcpy(&mg_windowDimension, pWindowBorders, sizeof(SDL_Rect));

    createShip(eEnemyShip, 0, 0);
    createShip(eEnemyShip, 5, 10);
    createShip(eEnemyShip, 34, 21);
    createShip(eEnemyShip, 65, 25);

}

static T_GfhHandle getGfxStruct(SDL_Renderer* pRenderer, E_ShipType type)
{
    T_GfhHandle ret;

    switch (type)
    {
        case ePlayerShip:
            ret.pGfxSurface = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/DETAILED_PLAYER.png");
            break;
        case eEnemyShip:
            ret.pGfxSurface = IMG_Load("/home/szczerbiakko/SpaceInvaders/SpaceInvaders/gfx/ENEMY.png");
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

static T_Ship* createShip(E_ShipType type, int xPos, int yPos)
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

    ShipsList_PushBack(&mg_Enemies, new);
}