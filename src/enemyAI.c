#include "utilities.h"
#include "enemyAI.h"
#include "commonShip.h"

static pthread_t mg_enemyAIthread = 0;

_Noreturn static void* EnemyAI_Task(void* arg);
void EnemyAI_MoveLeftToRightThenStepDown(T_Ship* pShip);
static bool leftBorderReached(T_Ship* pShip);
static bool rightBorderReached(T_Ship* pShip);

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
        if (loopsCnt >= 200)
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
    /*get borders*/
    if (leftBorderReached(pShip) && eDirectionLeft == pShip->mvmntDir)
    {
        pShip->borderDirReached = eDirectionLeft;
        pShip->initialY = pShip->shipHitbox.y;
        pShip->mvmntDir = eDirectionDown;
    }
    else if (rightBorderReached(pShip) && eDirectionRight == pShip->mvmntDir)
    {
        pShip->borderDirReached = eDirectionRight;
        pShip->initialY = pShip->shipHitbox.y;
        pShip->mvmntDir = eDirectionDown;
    }

    if (pShip->shipHitbox.y - pShip->initialY >= pShip->shipHitbox.h)
    {
        if (pShip->borderDirReached ==  eDirectionLeft)
        {
            pShip->mvmntDir = eDirectionRight;
        }
        else if (pShip->borderDirReached ==  eDirectionRight)
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