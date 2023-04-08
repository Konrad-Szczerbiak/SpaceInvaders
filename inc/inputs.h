//
// Created by szczerbiakko on 11.12.22.
//

#pragma once

#include <stdint.h>

void InputsModuleInit(void* playersArrayPtr);

void InputsModule_GeneralHandler(void* players[2]);
void InputsModule_Player1InputsHandling(void* eventInstancePtr, void* playerPtr);
