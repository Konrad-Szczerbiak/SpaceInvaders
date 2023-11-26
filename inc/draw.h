/*
 * This module allows for generating a list of objects to be displayed.
 * It also performs drawing operations, and operations on renderer
 * */
#include "utilities.h"
typedef enum {
    UpBorder,
    LeftBorder,
    DownBorder,
    RightBorder,
} E_Border;


 E_OpResult Draw_ModuleInit(void);

SDL_Renderer* Draw_GetRendererPtr(void);
bool Draw_isMvmntBorderReached(E_Border borderType, int positionValue);

int MovePlayerLeft(void);
int MovePlayerRight(void);
int MovePlayerUp(void);
int MovePlayerDown(void);

void setShootingtrue(void);
void setEnemyShootingtrue(void);
