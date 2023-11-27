//
// Created by szczerbiakko on 04.11.23.
//

#pragma once

typedef enum {
    UpBorder,
    LeftBorder,
    DownBorder,
    RightBorder,
} E_Border;

typedef enum {
    ePlayerShip,
    eEnemyShip,
    eShipTypesCnt
} E_ShipType;

typedef enum {
    eDirectionLeft,
    eDirectionRight,
    eDirectionDown = 0x04,
    eDirectionUp = 0x08
} E_MovementDirection;

typedef struct {
    SDL_Texture* lasTexture;
    SDL_Rect lasLeft;
    SDL_Rect lasRight;
} T_Laser;

typedef struct {
    SDL_Rect mvmntBorder;
    SDL_Texture* shipTexture;
    SDL_Rect shipHitbox;
    T_Laser lasers;
    bool isHit;
    bool isShooting;
    double renderAngle;
    int initialY;
    int speed;
    E_MovementDirection mvmntDir;
    E_MovementDirection borderDirReached;
} T_Ship;

typedef struct S_ShipsListElement{
    T_Ship* ship;
    struct S_ShipsListElement* pNext;
    struct S_ShipsListElement* pPrev;
} T_ShipsListElement;

typedef struct {
    T_ShipsListElement* head;
    T_ShipsListElement* tail;
    int count;
} T_ShipsList;

/**
 *
 * @param pRenderer
 * @param pScreenInfo
 * @param pWindowBorders
 */
void CommonShip_InitModule(SDL_Renderer* pRenderer, void* pScreenInfo, SDL_Rect* pWindowBorders);

/**
 *
 * @param pList
 * @param index
 * @return
 */
const T_Ship* ShipList_GetElementShipPtrByIndex(const T_ShipsList * const pList, int index);

/**
 *
 * @param pList
 * @param pShip
 * @return
 */
int ShipList_GetElementIndexByShipPtr(const T_ShipsList * const pList, const T_Ship* const pShip);

/**
 *
 * @param type
 * @param xPos
 * @param yPos
 * @return
 */
T_Ship* CommonShip_CreateShip(E_ShipType type, int xPos, int yPos);

/**
 *
 * @param type
 * @param pShip
 */
void CommonShip_DeleteShip(E_ShipType type, T_Ship* pShip);

/**
 *
 * @param pList
 * @param pCb
 */
void ShipList_PerformForEach(T_ShipsList* pList, void(*pCb)(T_Ship*));

/**
 *
 * @param type
 * @return
 */
T_ShipsList* CommonShip_GetShipListPtr(E_ShipType type);

/**
 *
 * @param pShip
 */
void CommonShip_RenderShip(T_Ship* pShip);

bool CommonShip_isMvmntBorderReached(SDL_Rect* pHitbox, E_Border borderType);

void CommonShip_Move(E_MovementDirection dir, T_Ship* pShip);
void CommonShip_MovePlayer(E_MovementDirection dir, int playerNr);
