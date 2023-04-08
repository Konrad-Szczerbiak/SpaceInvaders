//
// Created by szczerbiakko on 11.12.22.
//

#include "draw.h"
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>

static E_OpResult renderObject(void* rendererPtr, void* texture, void* hitboxPtr);

void Draw_GenerateRendererVisuals(void* rendererPtr, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    SDL_SetRenderDrawColor((SDL_Renderer*)rendererPtr, red, green, blue, alpha);
    SDL_RenderClear((SDL_Renderer*)rendererPtr);
}

void Draw_PresentGeneratedVisuals(void* rendererPtr)
{
    SDL_RenderPresent(rendererPtr);
}

static void* _SG_PlayerTexturePtr = NULL;
static void* _SG_EnemyTexturePtr = NULL;
static void* _SG_leftShotTexturePtr = NULL;
static void* _SG_rightShotTexturePtr = NULL;

E_OpResult setTexture(void* texturePtr, E_TextureType type)
{
    E_OpResult retval = eFailure;
    if (texturePtr != NULL)
    {
        switch (type)
        {
            case ePlayer:
                _SG_PlayerTexturePtr = texturePtr;
                break;
            case eEnemy:
                _SG_EnemyTexturePtr = texturePtr;
                break;
            case eLeftShot:
                _SG_leftShotTexturePtr = texturePtr;
                break;
            case eRightShot:
                _SG_rightShotTexturePtr = texturePtr;
                break;
        }
        retval = eSuccess;
    }
    return retval;
}

void* getTexture(E_TextureType type)
{
    switch (type)
    {
        case ePlayer:
            return _SG_PlayerTexturePtr;
        case eEnemy:
            return _SG_EnemyTexturePtr;
        case eLeftShot:
            return _SG_leftShotTexturePtr;
        case eRightShot:
            return _SG_rightShotTexturePtr;
    }
}


void* Draw_LoadTexture(void* rendererPtr, const char* filename)
{
    SDL_Texture* texturePtr;

    texturePtr = IMG_LoadTexture(rendererPtr, filename);

    return (void*)texturePtr;
}

void* Draw_CreateTextureFieldRectangle(int xPos, int yPos, int width, int height)
{
    SDL_Rect* newReturnedRectangularField = NULL;
    if ((newReturnedRectangularField = (SDL_Rect*)calloc(1, sizeof(SDL_Rect))) == NULL )
    {
        exit(99);
    }
    else
    {
        newReturnedRectangularField->w = width;
        newReturnedRectangularField->h = height;
        newReturnedRectangularField->x = xPos;
        newReturnedRectangularField->y = yPos;
    }

    return (void*)newReturnedRectangularField;
}

void Draw_DeleteTextureFieldRectangle(void* textureFieldPtr)
{
    free(textureFieldPtr);
}

static E_OpResult renderObject(void* rendererPtr, void* texture, void* hitboxPtr)
{
    E_OpResult retval = eFailure;
    const char* SDL_Err = NULL;
    SDL_Rect hitbox = *((SDL_Rect*) hitboxPtr);

    int width = hitbox.w;
    int height = hitbox.h;

    if ((retval = SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, &width, &height)) == eSuccess )
    {
        if ((retval = SDL_RenderCopy(rendererPtr, texture, NULL, &hitbox)) == eSuccess)
        {
            retval = eSuccess;
            return retval;
        }
        else
        {
            SDL_Err = SDL_GetError();
            printf("\n%s\n", SDL_Err);
        }
    }
    else
    {
        SDL_Err = SDL_GetError();
        printf("\n%s\n", SDL_Err);
    }
    return retval;
}

static void* _SG_playerHitboxRectPtr = NULL;
static void* _SG_leftShotRectPtr = NULL;
static void* _SG_rightShotRectPtr = NULL;

void setHitbox(void* textureFieldPtr, E_TextureType type)
{
    switch (type)
    {
        case ePlayer:
            _SG_playerHitboxRectPtr = textureFieldPtr;
            break;
        case eLeftShot:
            _SG_leftShotRectPtr = textureFieldPtr;
            break;
        case eRightShot:
            _SG_rightShotRectPtr = textureFieldPtr;
            break;
    }
}

void* getHitbox(E_TextureType type)
{
    switch (type)
    {
        case ePlayer:
            return _SG_playerHitboxRectPtr;
        case eLeftShot:
            return _SG_leftShotRectPtr;
        case eRightShot:
            return _SG_rightShotRectPtr;
    }
}

#include "entities.h"

E_OpResult RenderingModule_RenderShip(void* pShip)
{
    E_OpResult retval = eSuccess;

    T_Ship* ship = (T_Ship*)pShip;

    pthread_mutex_lock(ship->mutex);
    retval |= RenderingModule_RenderEntity(ship->shipPtr);
    retval |= RenderingModule_RenderEntity(ship->leftLaserPtr);
    retval |= RenderingModule_RenderEntity(ship->rightLaserPtr);
    pthread_mutex_unlock(ship->mutex);
    return retval;
}

E_OpResult RenderingModule_RenderEntity(void* pEntity)
{
    T_Entity* entityPtr = (T_Entity*)pEntity;

    if (EntitiesModule_AreFlagsSet(entityPtr, ENTITY_FLAG_VISIBILITY) == eSuccess)
    {
        if (renderObject(getRendererPtr(), entityPtr->texturePtr, entityPtr->hitboxPtr) == eSuccess)
        {
            return eSuccess;
        }
        else
        {
            return eFailure;
        }
    }
    return eUnnecessary;
}

_Noreturn void RenderingModule_RenderTask(__attribute__((unused))void* arg0)
{
    timer_t rendererTimer;
    clockid_t cpuClockID = 0;

    pthread_getcpuclockid(pthread_self(), &cpuClockID);

    if (timer_create(cpuClockID, NULL, &rendererTimer) != 0)
    {
        exit(37);
    }

    while(1)
    {

        Draw_GenerateRendererVisuals(getRendererPtr(), 45, 150, 200, 255);
//
//        /// to co wczesniej, ale bardziej zmodułować
//        RenderingModule_RenderShip(PLAYER);
//        RenderingModule_RenderShip(ENEMY1);
//        RenderingModule_RenderShip(ENEMY2);
//        RenderingModule_RenderShip(ENEMY3);
//        RenderingModule_RenderShip(ENEMY4);

        Draw_PresentGeneratedVisuals(getRendererPtr());

        sleep(40);
    }
}