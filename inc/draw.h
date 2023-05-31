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

E_OpResult RenderingModule_Init(void* arg0);

void RenderingModule_GenerateRendererVisuals(void* rendererPtr, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void RenderingModule_PresentGeneratedVisuals(void* rendererPtr);
void* RenderingModule_LoadTexture(void* rendererPtr, const char* filename);
E_OpResult Draw_RenderActualTexture(void* rendererPtr, void* texture, void* textureFieldPtr);

void* RenderingModule_CreateTextureFieldRectangle(int xPos, int yPos, int width, int height);
void RenderingModule_DeleteTextureFieldRectangle(void* textureFieldPtr);

E_OpResult  RenderingModule_SetTexture(void* textureFieldPtr, E_TextureType type);
void* RenderingModule_GetTexture(E_TextureType type);

void RenderingModule_SetHitbox(void* textureFieldPtr, E_TextureType type);
void* RenderingModule_GetHitbox(E_TextureType type);

E_OpResult RenderingModule_RenderShip(void* pShip);
E_OpResult RenderingModule_RenderEntity(void* pEntity);