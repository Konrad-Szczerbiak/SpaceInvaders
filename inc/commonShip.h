//
// Created by szczerbiakko on 04.11.23.
//

#pragma once

typedef enum {
    ePlayerShip,
    eEnemyShip,
    eShipTypesCnt
} E_ShipType;

void CommonShip_InitModule(SDL_Renderer* pRenderer, void* pScreenInfo);