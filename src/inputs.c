/*We create basic  inputs module here
 *
 * It will handle Keyboard Events, based on buttons clicked.
 * */

#include "utilities.h"
#include "pthread.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "draw.h"

static SDL_Event Event = {0}; /*General Event Structure*/

static SDL_KeyboardEvent KeyboardEvent = {
        0, /* type - "key down" or "key up" */
        0, /* timestamp with SDL_GetTicks */
        0, /* window ID with keyboard focus */
        0, /* state if pressed or released */
        0, /* non-zero if key repeated */
        0, /*  */
        0, /*  */
        0 /* keysym - id of key pressed */
};

static pthread_t InputsThread = 0;
static pthread_attr_t InputsThreadParams = {0};

_Noreturn static T_ThreadFunc InputsTask(void* arg);

void Inputs_ModuleInit(void)
{
    pthread_create(&InputsThread, &InputsThreadParams, InputsTask, NULL);
}

_Noreturn static T_ThreadFunc InputsTask(void* arg)
{
    THREAD_SLEEP(100); /*Allow for initial events pumping*/
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
                        incrementPlayerXCoord();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                    case SDLK_LEFT:
                        decrementPlayerXCoord();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                    case SDLK_DOWN:
                        incrementPlayerYCoord();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                    case SDLK_UP:
                        decrementPlayerYCoord();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); /*Flush the rest of events*/
                        break;
                }
            }
        }
        THREAD_SLEEP(1);
    }
}