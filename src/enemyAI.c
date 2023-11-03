#include "utilities.h"
#include "enemyAI.h"
#include "draw.h"

typedef enum
{
    eDirectionLeft,
    eDirectionRight,
    eDirectionDown = 0x04,
} E_MovementDirection;

typedef struct {
    T_Ship* ship;
    E_MovementDirection dir;
} T_ShipMovement;

static pthread_t mg_enemyAIthread = 0;

T_ShipMovement mg_enemyMvmnt = {0};
_Noreturn static void* EnemyAI_Task(void* arg);
void EnemyAI_MoveLeftToRightThenStepDown(T_ShipMovement* pMvmnt);
static bool leftBorderReached(T_Ship* pShip);
static bool rightBorderReached(T_Ship* pShip);
////////////////////////////////////////////////////////////////////////
void EnemyAI_Init(void)
{
    Draw_CreateEnemy(20, 20);

    mg_enemyMvmnt.ship = getEnemyPtr();
    pthread_create(&mg_enemyAIthread, NULL, EnemyAI_Task, &mg_enemyMvmnt);
}

_Noreturn static void* EnemyAI_Task(void* arg)
{
    int loopsCnt = 0;
    T_ShipMovement* pMvmnt = (T_ShipMovement*)(arg);
    while(1)
    {
        loopsCnt++;
        if (loopsCnt >= 20)
        {
            setEnemyShootingtrue();
            loopsCnt = 0;
        }
        EnemyAI_MoveLeftToRightThenStepDown(pMvmnt);
        ThreadSleep(10);
    }
}

void EnemyAI_MoveLeftToRightThenStepDown(T_ShipMovement* pMvmnt)
{
    static E_MovementDirection borderDirReached = 0xff;
    static int initialY = 0;
    /*get borders*/
    if (leftBorderReached(pMvmnt->ship) && eDirectionLeft == pMvmnt->dir)
    {
        borderDirReached = eDirectionLeft;
        initialY = pMvmnt->ship->playerHitbox.y;
        pMvmnt->dir = eDirectionDown;
    }
    else if (rightBorderReached(pMvmnt->ship) && eDirectionRight == pMvmnt->dir)
    {
        borderDirReached = eDirectionRight;
        initialY = pMvmnt->ship->playerHitbox.y;
        pMvmnt->dir = eDirectionDown;
    }

    if (pMvmnt->ship->playerHitbox.y - initialY >= pMvmnt->ship->playerHitbox.h)
    {
        if (borderDirReached ==  eDirectionLeft)
        {
            pMvmnt->dir = eDirectionRight;
        }
        else if (borderDirReached ==  eDirectionRight)
        {
            pMvmnt->dir = eDirectionLeft;
        }
    }

    switch (pMvmnt->dir)
    {
        case eDirectionRight:
            pMvmnt->ship->playerHitbox.x += 1;
            break;
        case eDirectionLeft:
            pMvmnt->ship->playerHitbox.x -= 1;
            break;
        case eDirectionDown:
            pMvmnt->ship->playerHitbox.y += 1;
            break;
        default:
            break;
    }
}

static bool rightBorderReached(T_Ship* pShip)
{
    return (pShip->playerHitbox.x >= pShip->mvmntBorder.w);
}

static bool leftBorderReached(T_Ship* pShip)
{
    return (pShip->playerHitbox.x <= pShip->mvmntBorder.x);
}