#include "utilities.h"
#include "enemyAI.h"
#include "commonShip.h"
//
//typedef enum
//{
//    eDirectionLeft,
//    eDirectionRight,
//    eDirectionDown = 0x04,
//} E_MovementDirection;

//typedef struct {
//    T_Ship* ship;
//    E_MovementDirection dir;
//} T_ShipMovement;

static pthread_t mg_enemyAIthread = 0;

//T_ShipMovement mg_enemyMvmnt = {0};
_Noreturn static void* EnemyAI_Task(void* arg);
void EnemyAI_MoveLeftToRightThenStepDown(T_Ship* pShip);
static bool leftBorderReached(T_Ship* pShip);
static bool rightBorderReached(T_Ship* pShip);
////////////////////////////////////////////////////////////////////////
void EnemyAI_Init(void)
{
    pthread_create(&mg_enemyAIthread, NULL, EnemyAI_Task, NULL);
}

_Noreturn static void* EnemyAI_Task(void* arg)
{
    T_ShipsList* pEnemyList = NULL;
    pEnemyList = CommonShip_GetShipListPtr(eEnemyShip);
    int loopsCnt = 0;
    while(1)
    {
        loopsCnt++;
        if (loopsCnt >= 1000)
        {
            if (pEnemyList->count < 10)
            {
                CommonShip_CreateShip(eEnemyShip, 0, 0);
            }
            else
            {
                CommonShip_DeleteShip(eEnemyShip, pEnemyList->head->ship);
            }
            //            setEnemyShootingtrue();
            loopsCnt = 0;
        }
        ShipList_PerformForEach(pEnemyList, EnemyAI_MoveLeftToRightThenStepDown);
        ThreadSleep(1);
    }
}

void EnemyAI_MoveLeftToRightThenStepDown(T_Ship* pShip)
{
    static E_MovementDirection borderDirReached = 0xff;
    /*get borders*/
    if (leftBorderReached(pShip) && eDirectionLeft == pShip->mvmntDir)
    {
        borderDirReached = eDirectionLeft;
        pShip->initialY = pShip->shipHitbox.y;
        pShip->mvmntDir = eDirectionDown;
    }
    else if (rightBorderReached(pShip) && eDirectionRight == pShip->mvmntDir)
    {
        borderDirReached = eDirectionRight;
        pShip->initialY = pShip->shipHitbox.y;
        pShip->mvmntDir = eDirectionDown;
    }

    if (pShip->shipHitbox.y - pShip->initialY >= pShip->shipHitbox.h)
    {
        if (borderDirReached ==  eDirectionLeft)
        {
            pShip->mvmntDir = eDirectionRight;
        }
        else if (borderDirReached ==  eDirectionRight)
        {
            pShip->mvmntDir = eDirectionLeft;
        }
    }

    switch (pShip->mvmntDir)
    {
        case eDirectionRight:
            pShip->shipHitbox.x += 1;
            break;
        case eDirectionLeft:
            pShip->shipHitbox.x -= 1;
            break;
        case eDirectionDown:
            pShip->shipHitbox.y += 1;
            break;
        default:
            break;
    }
}

static bool rightBorderReached(T_Ship* pShip)
{
    return (pShip->shipHitbox.x >= pShip->mvmntBorder.w);
}

static bool leftBorderReached(T_Ship* pShip)
{
    return (pShip->shipHitbox.x <= pShip->mvmntBorder.x);
}