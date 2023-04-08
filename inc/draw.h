//
// Created by szczerbiakko on 11.12.22.
//
#pragma once

#include <stdint.h>
#include "SDL_Init.h"

typedef enum {
    ePlayer,
    eLeftShot,
    eRightShot,
    eEnemy
} E_TextureType;

#define BLASTER_WIDTH_PX 7
#define BLASTER_START_OFFSET_PX 45

#define SPACESHIP_ENEMY_WIDTH_PX 85
#define SPACESHIP_ENEMY_HEIGHT_PX 115

void Draw_GenerateRendererVisuals(void* rendererPtr, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void Draw_PresentGeneratedVisuals(void* rendererPtr);
void* Draw_LoadTexture(void* rendererPtr, const char* filename);
E_OpResult Draw_RenderActualTexture(void* rendererPtr, void* texture, void* textureFieldPtr);

void* Draw_CreateTextureFieldRectangle(int xPos, int yPos, int width, int height);
void Draw_DeleteTextureFieldRectangle(void* textureFieldPtr);

E_OpResult  setTexture(void* textureFieldPtr, E_TextureType type);
void* getTexture(E_TextureType type);

void setHitbox(void* textureFieldPtr, E_TextureType type);
void* getHitbox(E_TextureType type);

E_OpResult RenderingModule_RenderShip(void* pShip);
E_OpResult RenderingModule_RenderEntity(void* pEntity);