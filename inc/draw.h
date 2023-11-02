/*
 * This module allows for generating a list of objects to be displayed.
 * It also performs drawing operations, and operations on renderer
 * */
#include "utilities.h"

typedef struct {
    SDL_Rect mvmntBorder;
    SDL_Texture* playerTexture;
    SDL_Rect playerHitbox;
} T_Ship;


 E_OpResult Draw_ModuleInit(void);

 T_Ship* getEnemyPtr(void);

void Draw_CreateEnemy(int x, int y);

int MovePlayerLeft(void);
int MovePlayerRight(void);
int MovePlayerUp(void);
int MovePlayerDown(void);
