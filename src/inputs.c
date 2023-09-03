/*We create basic  inputs module here
 *
 * It will handle Keyboard Events, based on buttons clicked.
 * */

#include "utilities.h"
#include "draw.h"

static SDL_Event Event = {0}; /*General Event Structure*/
static SDL_KeyboardEvent KeyboardEvent = {0};

static pthread_t InputsThread = 0;
static pthread_attr_t InputsThreadParams = {0};

_Noreturn static T_ThreadFunc InputsTask(void* arg);

void Inputs_ModuleInit(void)
{
    pthread_create(&InputsThread, &InputsThreadParams, InputsTask, NULL);
}

_Noreturn static T_ThreadFunc InputsTask(void* arg)
{
    ThreadSleep(100); /*Allow for initial events pumping*/
    while(1)
    {
        while (SDL_PollEvent(&Event) > 0)
        {
            if (SDL_KEYDOWN == Event.type || SDL_KEYUP == Event.type)
            {
                KeyboardEvent = Event.key;
                switch (KeyboardEvent.keysym.sym)
                {
                    case SDLK_RIGHT:
                        MovePlayerRight();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                    case SDLK_LEFT:
                        MovePlayerLeft();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                    case SDLK_DOWN:
                        MovePlayerDown();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                    case SDLK_UP:
                        MovePlayerUp();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                }
            }
        }
        ThreadSleep(1);
    }
}