/*
 * This module allows for generating a list of objects to be displayed.
 * It also performs drawing operations, and operations on renderer
 * */
#include "utilities.h"

//typedef struct {
//    SDL_Texture* lasTexture;
//    SDL_Rect lasLeft;
//    SDL_Rect lasRight;
//} T_Laser;
//
//typedef struct {
//    SDL_Rect mvmntBorder;
//    SDL_Texture* playerTexture;
//    SDL_Rect playerHitbox;
//    T_Laser lasers;
//} T_Ship;

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
