/*
 * Module containing common utilities
 */
#pragma once

#include <pthread.h>
#include "stdint.h"
#include "assert.h"
#include "SDL2/SDL.h"

void ThreadSleep(int ms);

typedef void* T_ThreadFunc;

typedef union {
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
    uint32_t alphahex;
} T_HexColor;

typedef enum {
    eSuccess,
    eFailure,
    eUnnecessary
} E_OpResult;

void Utilities_ConfigureErrorHandling(void);