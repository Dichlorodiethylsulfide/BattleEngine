#include "BEEngineLoop.h"
#include "SDL2/SDL_events.h"

void BECPULoop::EngineTick()
{
    SDL_Event Event{};
    while(SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_KEYDOWN)
        {
            if (Event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_tickState = BETickState::Stopped;
                break;
            }
        }
    }
}

void BEGPULoop::EngineTick()
{
    
}
