#pragma once
#include "SDL_Init.h"
#include "draw.h"

E_OpResult Collision_Detect(void* object1, void* object2);
void Collision_GenerateExplosion(E_TextureType type);