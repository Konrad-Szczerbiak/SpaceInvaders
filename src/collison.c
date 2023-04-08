//
// Created by szczerbiakko on 24.12.22.
//

#include "collison.h"
#include "SDL2/SDL.h"

E_OpResult Collision_Detect(void* object1, void* object2)
{
    E_OpResult retval = eFailure;
    SDL_Rect* hitbox1 = (SDL_Rect*)object1;
    SDL_Rect* hitbox2 = (SDL_Rect*)object2;
    if (SDL_HasIntersection(hitbox1, hitbox2))
    {
        retval = eSuccess;
    }
    return retval;
}

void Collision_GenerateExplosion(E_TextureType type)
{
    void* texture2BChanged = NULL;

    switch (type)
    {
        case ePlayer:
//            texture2BChanged = getTexture(ePlayer);
            setTexture(Draw_LoadTexture(getRendererPtr(), "gfx/explosion.png"), ePlayer);
            break;
        case eEnemy:
//            texture2BChanged = getTexture(eEnemy);
            setTexture(Draw_LoadTexture(getRendererPtr(), "gfx/explosion.png"), eEnemy);
            break;
    }
}