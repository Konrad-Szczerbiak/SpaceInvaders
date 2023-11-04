//
// Created by szczerbiakko on 03.11.23.
//

#include "collision.h"
#include "pthread.h"

#define WITHIN_BOUNDARIES(loThld, val, hiThld) (((loThld) <= (val)) && ((hiThld) >= (val)))

/*simple collision detection mechanism*/
bool Collision_isCollisionDetected(SDL_Rect* hitboxRect1, SDL_Rect* hitboxRect2)
{
    bool isOverlapOnXaxis = false;
    bool isOverlapOnYaxis = false;

    /*check hitboxes overlap*/
    isOverlapOnXaxis = (WITHIN_BOUNDARIES((hitboxRect1->x), (hitboxRect2->x), (hitboxRect1->x + hitboxRect1->w))) || (WITHIN_BOUNDARIES((hitboxRect2->x), (hitboxRect1->x), (hitboxRect2->x + hitboxRect2->w)));
    isOverlapOnYaxis = (WITHIN_BOUNDARIES((hitboxRect1->y), (hitboxRect2->y), (hitboxRect1->y + hitboxRect1->h))) || (WITHIN_BOUNDARIES((hitboxRect2->y), (hitboxRect1->y), (hitboxRect2->y + hitboxRect2->h)));

    return isOverlapOnYaxis && isOverlapOnXaxis;
}