//
// Created by szczerbiakko on 03.11.23.
//

#pragma once

#include "SDL_Init.h"

void Collision_InitModule(void);
bool Collision_isCollisionDetected(SDL_Rect* hitboxRect1, SDL_Rect* hitboxRect2);
