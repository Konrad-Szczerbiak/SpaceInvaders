//
// Created by szczerbiakko on 25.12.22.
//

#pragma once

#include "SDL_Init.h"

#define ENTITY_FLAG_VISIBILITY (1 << 0)
#define ENTITY_FLAG_CUTE (1 << 1)

typedef enum {
    eEntityPlayer,
    eEntityEnemy,
    eEntityLeftLaserBlast,
    eEntityRightLaserBlast,
} E_Entity;

typedef enum {
    eShipPlayer,
    eShipEnemy,
} E_Ship;

typedef enum {
    eXCoord,
    eYCoord,
} E_HitboxCoords;

typedef struct {
    unsigned int hitboxInitXPos;
    unsigned int hitboxInitYPos;
    unsigned int hitboxWidth;
    unsigned int hitboxHeight;
    void* hitboxPtr;
} T_HitboxData;

typedef struct {
    void* rendererPtr;
    void* texturePtr;
    const char* textureFilename;
} T_TextureData;

typedef struct {
    void* hitboxPtr;
    void* texturePtr;
    E_Entity type;
    int ID;
    int flags;
} T_Entity;

typedef struct {
    T_Entity* shipPtr;
    T_Entity* leftLaserPtr;
    T_Entity* rightLaserPtr;
    uint8_t isShooting;
    uint8_t hasHitRightBorder;
    void* nextShipPtr;
    void* prevShipPtr;
    pthread_mutex_t* mutex;
} T_Ship;

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
);

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
);

typedef enum {
    eDirectionUp,
    eDirectionLeft,
    eDirectionRight,
    eDirectionDown,
} E_Direction;

typedef enum {
    eEnemy1,
    eEnemy2,
    eEnemy3,
    eEnemy4,
    eEnemiesCnt
} E_EnemyID;

T_Ship* EntitiesModule_GetEnemyShipPtr(E_EnemyID ID);

int EntitiesModule_GetEnemiesArray(T_Ship* arrPtr);

void EntitiesModule_SetEntityFlag(T_Entity* entityPtr, int flag);

void EntitiesModule_ResetEntityFlag(T_Entity* entityPtr, int flag);

E_OpResult EntitiesModule_AreFlagsSet(T_Entity* entityPtr, int flags);

int EntitiesModule_GetCoord(T_Entity* entityPtr, E_HitboxCoords coordType);

E_OpResult EntitiesModule_PerformIfShipHitboxExists(void* shipPtr, E_OpResult (*performFunc)(void*));

E_OpResult EntitiesModule_PerformIfShipTextureExists(void* shipPtr, E_OpResult (*performFunc)(void*));

E_OpResult EntitiesModule_EnableShipShooting(void* shipPtr);

E_OpResult EntitiesModule_DisableShipShooting(void* shipPtr);

E_OpResult EntitiesModule_HandleShipShooting(void* shipPtr);

E_OpResult EntitiesModule_MoveShip(void* shipPtr, E_Direction direction);