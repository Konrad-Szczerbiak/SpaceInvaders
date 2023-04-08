#include <SDL2/SDL.h>

#include "inputs.h"
#include "entities.h"
#include "SDL_Init.h"

#include <pthread.h>

void InputsModuleInit(void* playersArrayPtr);

_Noreturn static void* InputsModule_Task(void* arg0);
static void handlePlayer1Movement(SDL_Event eventInstance, T_Ship* PLAYER);
static void enablePlayerShooting(T_Ship* PLAYER);
static void disablePlayerShooting(T_Ship* PLAYER);
static uint8_t handlePlayer1Shooting(void* playerPtr);

void InputsModuleInit(void* playersArrayPtr)
{
    pthread_t InputsHandlingThread = {0};

    pthread_create(&InputsHandlingThread, NULL, InputsModule_Task, playersArrayPtr);
}

_Noreturn static void* InputsModule_Task(void* arg0)
{
    while(1)
    {
        /// nalezy zrobic mutexami to, bo przez wyscig nie mamy mozliwosci strzelania
        InputsModule_GeneralHandler(arg0);
        sleep(10);
    }
}

void InputsModule_GeneralHandler(void* players[2])
{
    SDL_Event event = {0};

    if (SDL_PollEvent(&event) > 0)
    {
        InputsModule_Player1InputsHandling(&event, players[0]);
    }

    handlePlayer1Shooting(players[0]);
    if (players[1] != NULL)
    {
        /// do func
    }
}

void InputsModule_Player1InputsHandling(void* eventInstancePtr, void* playerPtr)
{
    T_Ship* PLAYER = (T_Ship*)playerPtr;

    pthread_mutex_lock(PLAYER->mutex);
    SDL_Event* eventInstance = (SDL_Event*)eventInstancePtr;

    if (eventInstancePtr != NULL)
    {
        switch ((*eventInstance).type)
        {
            case SDL_KEYDOWN:
                switch ((*eventInstance).key.keysym.sym)
                {
                    case SDLK_UP:
                    case SDLK_RIGHT:
                    case SDLK_LEFT:
                    case SDLK_DOWN:
                        handlePlayer1Movement((*eventInstance), PLAYER);
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                enablePlayerShooting(PLAYER);
                break;
        }
    }
    pthread_mutex_unlock(PLAYER->mutex);
}

static void handlePlayer1Movement(SDL_Event eventInstance, T_Ship* PLAYER)
{
    switch (eventInstance.type)
    {
        case SDL_KEYDOWN:
            switch (eventInstance.key.keysym.sym)
            {
                case SDLK_UP:
                case SDLK_w:
                    EntitiesModule_MoveShip((void*)PLAYER, eDirectionUp);
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    EntitiesModule_MoveShip((void*)PLAYER, eDirectionLeft);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    EntitiesModule_MoveShip((void*)PLAYER, eDirectionRight);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    EntitiesModule_MoveShip((void*)PLAYER, eDirectionDown);
                    break;
            }
        default:
            break;
    }
}

static void enablePlayerShooting(T_Ship* PLAYER)
{
    PLAYER->isShooting = YES;
    EntitiesModule_SetEntityFlag(PLAYER->leftLaserPtr, ENTITY_FLAG_VISIBILITY);
    EntitiesModule_SetEntityFlag(PLAYER->rightLaserPtr, ENTITY_FLAG_VISIBILITY);
}

static void disablePlayerShooting(T_Ship* PLAYER)
{
    PLAYER->isShooting = NO;
    EntitiesModule_ResetEntityFlag(PLAYER->leftLaserPtr, ENTITY_FLAG_VISIBILITY);
    EntitiesModule_ResetEntityFlag(PLAYER->rightLaserPtr, ENTITY_FLAG_VISIBILITY);
}

static uint8_t handlePlayer1Shooting(void* playerPtr)
{
    T_Ship* PLAYER = (T_Ship*)playerPtr;

    pthread_mutex_lock(PLAYER->mutex);
    if (PLAYER->isShooting == YES)
    {
        if (EntitiesModule_GetCoord(PLAYER->leftLaserPtr, eYCoord) > 0 && EntitiesModule_GetCoord(PLAYER->rightLaserPtr, eYCoord) > 0 )
        {
            ((SDL_Rect*)((PLAYER->leftLaserPtr)->hitboxPtr))->y -= LASER_SINGLE_MVMT_STEP_Y; /// TODO: ZMIENIC NA PARAMETR
            ((SDL_Rect*)((PLAYER->rightLaserPtr)->hitboxPtr))->y -= LASER_SINGLE_MVMT_STEP_Y; /// TODO: ZMIENIC NA PARAMETR
        }
        else
        {
            ((SDL_Rect*)((PLAYER->leftLaserPtr)->hitboxPtr))->y = ((SDL_Rect*)((PLAYER->shipPtr)->hitboxPtr))->y;
            ((SDL_Rect*)((PLAYER->rightLaserPtr)->hitboxPtr))->y = ((SDL_Rect*)((PLAYER->shipPtr)->hitboxPtr))->y;
            disablePlayerShooting(PLAYER);
        }
    }
    pthread_mutex_unlock(PLAYER->mutex);
}