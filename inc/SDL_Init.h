//
// Created by szczerbiakko on 05.12.22.
//

#pragma once

#include <stdint.h>
#include <pthread.h>
#include "utilities.h"

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720

#define YES 0x01
#define NO 0x00

#define SHIP_SINGLE_MVMT_STEP_X 10
#define LASER_SINGLE_MVMT_STEP_Y 30

E_OpResult SDL2_Init(void);

void* getRendererPtr(void);

void GameQuit(void);