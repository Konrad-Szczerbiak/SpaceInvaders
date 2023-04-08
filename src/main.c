#include <stdio.h>
#include "SDL_Init.h"
#include "inputs.h"
#include "draw.h"
#include "collison.h"
#include <stdlib.h>
#include "entities.h"
#include "enemies.h"

#define ERR_CANNOT_LOAD_TEXTURE 69

void performAtExit(void)
{
    printf("COULD NOT LOAD TEXTURE FILE - ERRCODE: %d", ERR_CANNOT_LOAD_TEXTURE);
}

int main() {

    if (SDL2_Init() == eSuccess)
    {
        T_Ship* PLAYER = NULL;
        T_Ship* ENEMY1 = NULL;
        T_Ship* ENEMY2 = NULL;
        T_Ship* ENEMY3 = NULL;
        T_Ship* ENEMY4 = NULL;

        if (EntitiesModule_GenerateShip
        (
            getRendererPtr(),
            "gfx/smplTexture.png",
            "gfx/shot.png",
            300,
            300,
            85,
            115,
            9,
            15,
            eEntityPlayer,
            0,
            &PLAYER,
            NULL
        ) == eFailure)
        {
            exit(21);
        }

        if (EntitiesModule_GenerateShip
        (
                getRendererPtr(),
                "gfx/enemy.png",
                "gfx/shot.png",
                100,
                100,
                85,
                115,
                9,
                15,
                eEntityEnemy,
                0,
                &ENEMY1,
                NULL
        ) == eFailure)
        {
            exit(22);
        }

        if (EntitiesModule_GenerateShip
                    (
                            getRendererPtr(),
                            "gfx/enemy.png",
                            "gfx/shot.png",
                            300,
                            100,
                            85,
                            115,
                            9,
                            15,
                            eEntityEnemy,
                            0,
                            &ENEMY2,
                            &ENEMY1
                    ) == eFailure)
        {
            exit(22);
        }

        if (EntitiesModule_GenerateShip
                    (
                            getRendererPtr(),
                            "gfx/enemy.png",
                            "gfx/shot.png",
                            500,
                            100,
                            85,
                            115,
                            9,
                            15,
                            eEntityEnemy,
                            0,
                            &ENEMY3,
                            &ENEMY2
                    ) == eFailure)
        {
            exit(22);
        }

        if (EntitiesModule_GenerateShip
                    (
                            getRendererPtr(),
                            "gfx/enemy.png",
                            "gfx/shot.png",
                            700,
                            100,
                            85,
                            115,
                            9,
                            15,
                            eEntityEnemy,
                            0,
                            &ENEMY4,
                            &ENEMY3
                    ) == eFailure)
        {
            exit(22);
        }

        void* enemiesModuleArgVector[4] = {ENEMY1, ENEMY2, ENEMY3, ENEMY4};
        int enemiesModuleArgVectorLen = 4;
        void* PLAYERS_ARR[2] = {PLAYER, 0};

        InputsModuleInit(PLAYERS_ARR);
        EnemiesModuleInit(enemiesModuleArgVectorLen, enemiesModuleArgVector[0]);

        while(1)
        {
            ;;;
        }
//
//        void* rendererPtr = getRendererPtr();
//
//        void* playerTexturePtr = NULL;
//        void* playerHitboxPtr = NULL;
//
//        if (setTexture((playerTexturePtr = Draw_LoadTexture(rendererPtr, "gfx/smplTexture.png")), ePlayer) != eSuccess)
//        {
//            atexit(performAtExit);
//            exit(ERR_CANNOT_LOAD_TEXTURE);
//        }
//
//        void* enemyTexturePtr = NULL;
//        void* enemyHitboxPtr = NULL;
//
//        if (setTexture((enemyTexturePtr = Draw_LoadTexture(rendererPtr, "gfx/enemy.png")), eEnemy) != eSuccess)
//        {
//            atexit(performAtExit);
//            exit(ERR_CANNOT_LOAD_TEXTURE);
//        }
//
//        void* rightShotTexturePtr = NULL;
//        void* rightShotHitboxPtr = NULL;
//        void* leftShotTexturePtr = NULL;
//        void* leftShotHitboxPtr = NULL;
//
//        if (setTexture((rightShotTexturePtr = Draw_LoadTexture(rendererPtr, "gfx/shot.png")), eRightShot) == eFailure ||
//                setTexture((leftShotTexturePtr = Draw_LoadTexture(rendererPtr, "gfx/shot.png")), eLeftShot) == eFailure )
//        {
//            atexit(performAtExit);
//            exit(ERR_CANNOT_LOAD_TEXTURE);
//        }
//
//        struct ship {
//            int xPos;
//            int yPos;
//            int width;
//            int height;
//        } Ship = {200, 200, 85, 115}, Enemy;
//
//        Enemy.width = SPACESHIP_ENEMY_WIDTH_PX;
//        Enemy.height = SPACESHIP_ENEMY_HEIGHT_PX;
//        Enemy.xPos = (WINDOW_WIDTH - SPACESHIP_ENEMY_WIDTH_PX)/2;
//        Enemy.yPos = 100;
//
//        struct laser {
//            int xPos;
//            int yPos;
//            int width;
//            int height;
//        } leftLaser, rightLaser;
//        rightLaser.width = leftLaser.width = 9;
//        rightLaser.height = leftLaser.height = 20;
//        leftLaser.xPos = Ship.xPos - (leftLaser.width - BLASTER_WIDTH_PX)/2;
//        rightLaser.yPos = leftLaser.yPos = Ship.yPos + BLASTER_START_OFFSET_PX;
//        rightLaser.xPos = (Ship.xPos + Ship.width) - (BLASTER_WIDTH_PX + (rightLaser.width - BLASTER_WIDTH_PX)/2);
//
//        setHitbox(playerHitboxPtr = Draw_CreateTextureFieldRectangle(Ship.xPos,Ship.yPos,Ship.width,Ship.height), ePlayer);
//
//        setHitbox(enemyHitboxPtr = Draw_CreateTextureFieldRectangle(Enemy.xPos, Enemy.yPos, Enemy.width, Enemy.height), eEnemy);
//
//        /// szerokosc blastera to 7 px. pozycja 200 - 1 to lewa krawedz blastera. (x_statek) - ((w_laser - w_blaster)/2)
//        setHitbox(rightShotHitboxPtr = Draw_CreateTextureFieldRectangle(rightLaser.xPos,rightLaser.yPos,rightLaser.width,rightLaser.height), eRightShot);
//        /// szerokosc blastera to 7 px. pozycja 285 to prawa krawedz blastera. chcemy wysrodkowac laser, wiec: (x_statek + w_statek) - (w_blaster + ((w_laser -  w_blaster)/2))
//        setHitbox(leftShotHitboxPtr = Draw_CreateTextureFieldRectangle(leftLaser.xPos,leftLaser.yPos,leftLaser.width,leftLaser.height),eLeftShot);
//
//        //// ważne flagi inputu - definiuja co sie dzieje
//        uint8_t inputFlags = 0x00;
//
//        while (1)
//        {
//            playerTexturePtr = getTexture(ePlayer);
//            enemyTexturePtr = getTexture(eEnemy);
//
//            Draw_GenerateRendererVisuals(rendererPtr, 45, 150, 200, 255);
//
//            Inputs_SDLInputHandling(&inputFlags);
//
//            Draw_RenderActualTexture(rendererPtr, playerTexturePtr, playerHitboxPtr);
//            Draw_RenderActualTexture(rendererPtr, enemyTexturePtr, enemyHitboxPtr);
//
//            /// przy nacisnieciu myszki powinno wyswietlac  dwie wiazki lasera
//            /// nastepnie powinno usuwac je gdy y dotknie granicy
//            /// x powinien znajdowac sie mniej wiecej na linii blasterow statku
//            Draw_RenderActualTexture(rendererPtr, rightShotTexturePtr, rightShotHitboxPtr);
//            Draw_RenderActualTexture(rendererPtr, leftShotTexturePtr, leftShotHitboxPtr);
//
//            if (Collision_Detect(rightShotHitboxPtr, enemyHitboxPtr) == eSuccess || Collision_Detect(leftShotHitboxPtr, enemyHitboxPtr) == eSuccess)
//            {
//                Collision_GenerateExplosion(eEnemy);
//            }
//
//            Draw_PresentGeneratedVisuals(rendererPtr);
//
//            sleep(10);
//
//        }
    }
    else
    {
        printf("SDL WAS NOT ABLE TO INITIALISE");
    }
    return 0;
}
