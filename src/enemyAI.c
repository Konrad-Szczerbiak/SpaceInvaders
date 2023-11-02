#include "utilities.h"
#include "enemyAI.h"
#include "draw.h"

typedef enum
{
    eDirectionLeft,
    eDirectionRight,
} E_MovementDirection;

typedef struct {
    T_Ship* ship;
    E_MovementDirection dir;
} T_ShipMovement;

static pthread_t mg_enemyAIthread = 0;

T_ShipMovement mg_enemyMvmnt = {0};
_Noreturn static void* EnemyAI_Task(void* arg);
void EnemyAI_MoveLeftToRight(T_ShipMovement* pMvmnt);
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
    T_ShipMovement* pMvmnt = (T_ShipMovement*)(arg);
    while(1)
    {
        EnemyAI_MoveLeftToRight(pMvmnt);
        ThreadSleep(10);
    }
}

void EnemyAI_MoveLeftToRight(T_ShipMovement* pMvmnt)
{
    /*get borders*/
    if (leftBorderReached(pMvmnt->ship))
    {
        pMvmnt->dir = eDirectionRight;
    }
    else if (rightBorderReached(pMvmnt->ship))
    {
        pMvmnt->dir = eDirectionLeft;
    }
    switch (pMvmnt->dir)
    {
        case eDirectionLeft:
            pMvmnt->ship->playerHitbox.x += 1;
            break;
        case eDirectionRight:
            pMvmnt->ship->playerHitbox.x -= 1;
            break;
        default:
            break;
    }
}

static bool leftBorderReached(T_Ship* pShip)
{
    return (pShip->playerHitbox.x >= pShip->mvmntBorder.w);
}

static bool rightBorderReached(T_Ship* pShip)
{
    return (pShip->playerHitbox.x <= pShip->mvmntBorder.x);
}