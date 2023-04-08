//
// Created by szczerbiakko on 27.12.22.
//

#include "enemies.h"
#include "entities.h"
#include <SDL2/SDL.h>

#include <pthread.h>

#include <time.h>

typedef struct {
    int size;
    void* headPtr;
} T_ArgVector;

void EnemiesModuleInit(int argumentsVectorSize, void* argumentsVectorPtr);
_Noreturn static void* EnemiesModule_Task(void* argv);
static E_OpResult EnemiesModule_SimpleEnemyMovement(void* shipPtr);
static E_OpResult EnemiesModule_EnemyMovementWithShooting(void* shipPtr);

T_ArgVector mg_argv;

void EnemiesModuleInit(int argumentsVectorSize, void* argumentsVectorPtr)
{
    pthread_t enemiesModuleThread = 0;

    mg_argv.headPtr = argumentsVectorPtr;
    mg_argv.size = argumentsVectorSize;

    pthread_create(&enemiesModuleThread, NULL, EnemiesModule_Task, &mg_argv);
}

_Noreturn static void* EnemiesModule_Task(void* argv)
{
    T_ArgVector* EnemiesVector = argv;

    EnemiesVector = &mg_argv;

    T_Ship* currentEnemyPtr = NULL;

    while (1)
    {
        currentEnemyPtr = mg_argv.headPtr;
        for (int vectorIterator = 0; vectorIterator < EnemiesVector->size; vectorIterator++)
        {
            EntitiesModule_PerformIfShipHitboxExists(currentEnemyPtr, EnemiesModule_SimpleEnemyMovement);
            EntitiesModule_PerformIfShipHitboxExists(currentEnemyPtr, EnemiesModule_EnemyMovementWithShooting);
            currentEnemyPtr = currentEnemyPtr->nextShipPtr;
        }
        sleep(10);
    }
}

static E_OpResult EnemiesModule_SimpleEnemyMovement(void* shipPtr)
{
    T_Ship* ENEMY = (T_Ship*)shipPtr;

    if (ENEMY->hasHitRightBorder == NO)
    {
        EntitiesModule_MoveShip(shipPtr, eDirectionRight);
    }
    if ((((SDL_Rect*)(ENEMY->shipPtr->hitboxPtr))->x + ((SDL_Rect*)(ENEMY->shipPtr->hitboxPtr))->w) >= WINDOW_WIDTH && ENEMY->hasHitRightBorder == NO)
    {
        ENEMY->hasHitRightBorder = YES;
    }

    if (ENEMY->hasHitRightBorder == YES)
    {
        EntitiesModule_MoveShip(shipPtr, eDirectionLeft);
    }
    if (((SDL_Rect*)(ENEMY->shipPtr->hitboxPtr))->x <= 0 && ENEMY->hasHitRightBorder == YES)
    {
        ENEMY->hasHitRightBorder = NO;
    }
}

static E_OpResult EnemiesModule_EnemyMovementWithShooting(void* shipPtr)
{
    T_Ship* ENEMY = (T_Ship*)shipPtr;
    if (ENEMY != NULL)
    {

        static time_t timeEndShoot;
        time_t timeNow;

        time(&timeNow);
        if (ENEMY->isShooting == NO)
        {
            if (difftime(timeNow, timeEndShoot) >= 2)
            {
                EntitiesModule_PerformIfShipHitboxExists(shipPtr, EntitiesModule_EnableShipShooting);
                ENEMY->isShooting = YES;
            }
        }
        else
        {
            time(&timeEndShoot);
        }
        EntitiesModule_PerformIfShipHitboxExists(shipPtr, EntitiesModule_HandleShipShooting);

    }
}

