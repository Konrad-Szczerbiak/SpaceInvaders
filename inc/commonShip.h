//
// Created by szczerbiakko on 04.11.23.
//

#pragma once

typedef enum {
    ePlayerShip,
    eEnemyShip,
    eShipTypesCnt
} E_ShipType;

/**
 *
 * @param pRenderer
 * @param pScreenInfo
 * @param pWindowBorders
 */
void CommonShip_InitModule(SDL_Renderer* pRenderer, void* pScreenInfo, SDL_Rect* pWindowBorders);
