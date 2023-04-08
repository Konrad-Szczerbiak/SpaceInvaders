//
// Created by szczerbiakko on 25.12.22.
//

/*
 * Moduł dotyczy instancji obiektów
 * Ma on zapisane obiekty pojedyncze jak i złożone
 *
 * Obiekt pojedynczy - pojedynczy hitbox z teksturą
 * Obiekt złożony - obiekt składający się z kilku obiektów pojedynczych
 *
 * Powinienj zawierac:
 * 1. Funkcje do tworzenia obiektu pojedynczego
 * 2. Funkcje do zwracania wskaznika na obiekt pojedynczy
 *
 * 3. Funkcje do tworzenia obiektu złożonego
 * 4. Funkcje do zwracania wskaznika do obiektu złożonego.
 *
 *
 * */

/// ************* SDL2 ************* ///
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
/// ************* SDL2 ************* ///

/// ************* INIT ************* ///
#include "SDL_Init.h"
/// ************* INIT ************* ///

#include "entities.h"

/*** STATIC FUNCTIONS ***/
static T_TextureData* createTexture(T_TextureData* this);
static T_HitboxData* createHitbox(T_HitboxData* this);
static void* loadTexture(T_TextureData* new);
/*** STATIC FUNCTIONS ***/

static T_Ship* mg_enemiesArray[eEnemiesCnt] = {0};

T_Ship* getEnemyShipPtr(E_EnemyID ID)
{
    if (ID <= eEnemiesCnt)
    {
        return mg_enemiesArray[ID];
    }
    return NULL;
}

int getEnemiesArray(T_Ship* arrPtr)
{
    arrPtr = mg_enemiesArray;
    return (sizeof(mg_enemiesArray)/sizeof(void*));
}

E_OpResult EntitiesModule_GenerateWave(void)
{

}

E_OpResult EntitiesModule_GenerateShip(
        void* rendererPtr,
        const char* bodyTextureFilename,
        const char* laserTextureFilename,
        unsigned int shipInitialX,
        unsigned int shipInitialY,
        unsigned int shipWidth,
        unsigned int shipHeight,
        unsigned int laserWidth,
        unsigned int laserHeight,
        E_Entity type,
        int ID,
        T_Ship** SHIP,
        T_Ship** previousShipPtr
    )
{
    E_OpResult result = eFailure;

    T_Ship* new = (T_Ship*) calloc(1, sizeof(T_Ship));


    if (EntitiesModule_CreateEntity(
            rendererPtr,
            bodyTextureFilename,
            shipInitialX,
            shipInitialY,
            shipWidth,
            shipHeight,
            type,
            ID,
            &(new->shipPtr)
            ) == eSuccess)
    {
        EntitiesModule_SetEntityFlag(new->shipPtr, ENTITY_FLAG_VISIBILITY);
        if (EntitiesModule_CreateEntity(
                rendererPtr,
                laserTextureFilename,
                shipInitialX,
                shipInitialY,
                laserWidth,
                laserHeight,
                eEntityLeftLaserBlast,
                ID,
                &(new->leftLaserPtr)
        ) == eSuccess)
        {
            if (EntitiesModule_CreateEntity(
                    rendererPtr,
                    laserTextureFilename,
                    shipInitialX+shipWidth-laserWidth,
                    shipInitialY,
                    laserWidth,
                    laserHeight,
                    eEntityRightLaserBlast,
                    ID,
                    &(new->rightLaserPtr)
            ) == eSuccess)
            {
                *SHIP = new;
                if (previousShipPtr != NULL)
                {
                    new->prevShipPtr = *previousShipPtr;
                    (*previousShipPtr)->nextShipPtr = (void*)new;
                }

                new->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
                pthread_mutex_init(new->mutex, NULL);
                result = eSuccess;
            }
        }
    }

    return result;
}

E_OpResult EntitiesModule_CreateEntity(
        void* rendererPtr,
        const char* textureFilename,
        unsigned int initialX,
        unsigned int initialY,
        unsigned int width,
        unsigned int height,
        E_Entity type,
        int ID,
        T_Entity** entityPtr
        )
{
    T_Entity* new;
    if ((new = (T_Entity*) calloc(1, sizeof(T_Entity))) != NULL)
    {
        /////////////////////////////////////////////////////////////////////////////////
        /// hitbox generation
        T_HitboxData* genHitbox = NULL;
        if ((genHitbox = (T_HitboxData*) calloc(1, sizeof(T_HitboxData))) != NULL)
        {
            genHitbox->hitboxInitXPos = initialX;
            genHitbox->hitboxInitYPos = initialY;
            genHitbox->hitboxWidth = width;
            genHitbox->hitboxHeight = height;
            genHitbox = createHitbox(genHitbox);
            new->hitboxPtr = genHitbox->hitboxPtr;
        }
        else
        {
            return eFailure;
        }
        /////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////
        /// texture generation
        T_TextureData* genTexture = NULL;
        if ((genTexture = (T_TextureData*) calloc(1, sizeof(T_TextureData))) != NULL)
        {
            genTexture->rendererPtr = rendererPtr;
            genTexture->textureFilename = textureFilename;
            genTexture = createTexture(genTexture);
            new->texturePtr = genTexture->texturePtr;
        }
        else
        {
            return eFailure;
        }
        /////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////
        /// assigning type
        new->type = type;
        /////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////
        /// assigning ID
        new->ID = ID;
        /////////////////////////////////////////////////////////////////////////////////

        /// load setup under a pointer
        *entityPtr = new;
        return eSuccess;
    }
    return eFailure;
}

static T_HitboxData* createHitbox(T_HitboxData* this)
{
    SDL_Rect* hitbox = (SDL_Rect*) calloc(1, sizeof(SDL_Rect));

    hitbox->x = this->hitboxInitXPos;
    hitbox->y = this->hitboxInitYPos;
    hitbox->w = this->hitboxWidth;
    hitbox->h = this->hitboxHeight;

    this->hitboxPtr = hitbox;

    return this;
}

static T_TextureData* createTexture(T_TextureData* this)
{
    if ((this->rendererPtr != NULL) && (this->textureFilename != NULL))
    {
        this->texturePtr = loadTexture(this);
    }
    else if (this->texturePtr != NULL)
    {
        this->texturePtr = NULL;
    }

    return this;
}

static void* loadTexture(T_TextureData* new)
{
    return (void*)(IMG_LoadTexture(new->rendererPtr, new->textureFilename));
}

void EntitiesModule_SetEntityFlag(T_Entity* entityPtr, int flags)
{
    (entityPtr->flags) |= flags;
}

void EntitiesModule_ResetEntityFlag(T_Entity* entityPtr, int flags)
{
    (entityPtr->flags) ^= flags;
}

E_OpResult EntitiesModule_AreFlagsSet(T_Entity* entityPtr, int flags)
{
    return (entityPtr->flags & flags) ? eSuccess : eFailure;
}

int EntitiesModule_GetCoord(T_Entity* entityPtr, E_HitboxCoords coordType)
{
    switch (coordType)
    {
        case eXCoord:
            return ((SDL_Rect*)entityPtr->hitboxPtr)->x;
        case eYCoord:
            return ((SDL_Rect*)entityPtr->hitboxPtr)->y;
        default:
            return 0;
    }
}

E_OpResult EntitiesModule_PerformIfShipHitboxExists(void* shipPtr, E_OpResult (*performFunc)(void*))
{
    E_OpResult retval = eFailure;
    T_Ship* SHIP = (T_Ship*)shipPtr;
    if (SHIP->shipPtr != NULL)
    {
        if (SHIP->shipPtr->hitboxPtr != NULL)
        {
            retval = performFunc(shipPtr);
        }
    }
    return retval;
}

E_OpResult EntitiesModule_PerformIfShipTextureExists(void* shipPtr, E_OpResult (*performFunc)(void*))
{
    E_OpResult retval = eFailure;
    T_Ship* SHIP = (T_Ship*)shipPtr;
    if (SHIP->shipPtr != NULL)
    {
        if (SHIP->shipPtr->texturePtr != NULL)
        {
            retval = performFunc(shipPtr);
        }
    }
    return retval;
}

E_OpResult EntitiesModule_EnableShipShooting(void* shipPtr)
{
    if (shipPtr != NULL)
    {
        T_Ship* SHIP = (T_Ship*)shipPtr;

        SHIP->isShooting = YES;
        EntitiesModule_SetEntityFlag(SHIP->leftLaserPtr, ENTITY_FLAG_VISIBILITY);
        EntitiesModule_SetEntityFlag(SHIP->rightLaserPtr, ENTITY_FLAG_VISIBILITY);

        return eSuccess;
    }
    return eFailure;
}

E_OpResult EntitiesModule_DisableShipShooting(void* shipPtr)
{
    if (shipPtr != NULL)
    {
        T_Ship* SHIP = (T_Ship*)shipPtr;

        SHIP->isShooting = NO;
        EntitiesModule_ResetEntityFlag(SHIP->leftLaserPtr, ENTITY_FLAG_VISIBILITY);
        EntitiesModule_ResetEntityFlag(SHIP->rightLaserPtr, ENTITY_FLAG_VISIBILITY);

        return eSuccess;
    }
    return eFailure;
}

E_OpResult EntitiesModule_HandleShipShooting(void* shipPtr)
{
    if (shipPtr != NULL)
    {
        T_Ship* SHIP = (T_Ship*)shipPtr;
        pthread_mutex_lock(SHIP->mutex);
        if (SHIP->isShooting == YES)
        {
            if (((T_Entity*)(SHIP->shipPtr))->type == eEntityPlayer)
            {
                if (EntitiesModule_GetCoord(SHIP->leftLaserPtr, eYCoord) > 0 && EntitiesModule_GetCoord(SHIP->rightLaserPtr, eYCoord) > 0 )
                {
                    ((SDL_Rect*)((SHIP->leftLaserPtr)->hitboxPtr))->y -= LASER_SINGLE_MVMT_STEP_Y; /// TODO: ZMIENIC NA PARAMETR
                    ((SDL_Rect*)((SHIP->rightLaserPtr)->hitboxPtr))->y -= LASER_SINGLE_MVMT_STEP_Y; /// TODO: ZMIENIC NA PARAMETR
                }
                else
                {
                    ((SDL_Rect*)((SHIP->leftLaserPtr)->hitboxPtr))->y = ((SDL_Rect*)((SHIP->shipPtr)->hitboxPtr))->y;
                    ((SDL_Rect*)((SHIP->rightLaserPtr)->hitboxPtr))->y = ((SDL_Rect*)((SHIP->shipPtr)->hitboxPtr))->y;
                    EntitiesModule_DisableShipShooting(shipPtr);
                }
            }
            else if (((T_Entity*)(SHIP->shipPtr))->type == eEntityEnemy)
            {
                if (EntitiesModule_GetCoord(SHIP->leftLaserPtr, eYCoord) < WINDOW_HEIGHT && EntitiesModule_GetCoord(SHIP->rightLaserPtr, eYCoord) < WINDOW_HEIGHT )
                {
                    ((SDL_Rect*)((SHIP->leftLaserPtr)->hitboxPtr))->y += LASER_SINGLE_MVMT_STEP_Y; /// TODO: ZMIENIC NA PARAMETR
                    ((SDL_Rect*)((SHIP->rightLaserPtr)->hitboxPtr))->y += LASER_SINGLE_MVMT_STEP_Y; /// TODO: ZMIENIC NA PARAMETR
                }
                else
                {
                    ((SDL_Rect*)((SHIP->leftLaserPtr)->hitboxPtr))->y = ((SDL_Rect*)((SHIP->shipPtr)->hitboxPtr))->y;
                    ((SDL_Rect*)((SHIP->rightLaserPtr)->hitboxPtr))->y = ((SDL_Rect*)((SHIP->shipPtr)->hitboxPtr))->y;
                    EntitiesModule_DisableShipShooting(shipPtr);
                }
            }
        }
        pthread_mutex_unlock(SHIP->mutex);
        return eSuccess;
    }
    return eFailure;
}

E_OpResult EntitiesModule_MoveShip(void* shipPtr, E_Direction direction)
{
    if (shipPtr != NULL)
    {
        T_Ship* SHIP = (T_Ship*)shipPtr;

        if (SHIP->shipPtr != NULL)
        {
            if (SHIP->shipPtr->hitboxPtr != NULL)
            {
                switch (direction)
                {
                    case eDirectionUp:
                        if (((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->y > 0)
                        {
                            ((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->y -= SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->leftLaserPtr)->hitboxPtr))->y -= SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->rightLaserPtr)->hitboxPtr))->y -= SHIP_SINGLE_MVMT_STEP_X;
                        }
                        break;
                    case eDirectionLeft:
                        if (((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->x > 0)
                        {
                            ((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->x -= SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->leftLaserPtr)->hitboxPtr))->x -= SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->rightLaserPtr)->hitboxPtr))->x -= SHIP_SINGLE_MVMT_STEP_X;
                        }
                        break;
                    case eDirectionRight:
                        if ((((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->x + ((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->w) < WINDOW_WIDTH)
                        {
                            ((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->x += SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->leftLaserPtr)->hitboxPtr))->x += SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->rightLaserPtr)->hitboxPtr))->x += SHIP_SINGLE_MVMT_STEP_X;
                        }
                        break;
                    case eDirectionDown:
                        if ((((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->y + ((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->h) < WINDOW_HEIGHT)
                        {
                            ((SDL_Rect *)((SHIP->shipPtr)->hitboxPtr))->y += SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->leftLaserPtr)->hitboxPtr))->y += SHIP_SINGLE_MVMT_STEP_X;
                            ((SDL_Rect *)((SHIP->rightLaserPtr)->hitboxPtr))->y += SHIP_SINGLE_MVMT_STEP_X;
                        }
                        break;
                }
            }
        }

        return eSuccess;
    }
    return eFailure;
}