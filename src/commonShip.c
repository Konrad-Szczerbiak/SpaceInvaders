//
// Created by szczerbiakko on 04.11.23.
//

#include "utilities.h"
#include "commonShip.h"

typedef struct {
    SDL_Texture* lasTexture;
    SDL_Rect lasLeft;
    SDL_Rect lasRight;
} T_Laser;

typedef struct {
    SDL_Rect mvmntBorder;
    SDL_Texture* playerTexture;
    SDL_Rect playerHitbox;
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

T_Ship* commonShip_CreateShip()

/*currently used display representation as a rectangle*/
static SDL_Rect mg_windowDimension = {0};

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
void CommonShip_InitModule(void* pScreenInfo)
{

}
