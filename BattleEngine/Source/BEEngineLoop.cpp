#include "BEEngineLoop.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"

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
    FOREACH(m_tickableObjects)
    {
        const_cast<BEVector<BETickable>::Ref>(FOREACH_VALUE).Tick();
    }
}

BEGPULoop::BEGPULoop(External WindowPointer)
    : BERenderer(WindowPointer)
{
    auto renderable = BERenderable();
    renderable.LoadTexture(*this, "C:\\Users\\rikil\\Desktop\\Block.png", {{0, 0}, {50, 50}});
    m_renderableObjects.PushBack(std::move(renderable));
}

void BEGPULoop::EngineTick()
{
    ClearWindow();
    FOREACH(m_renderableObjects)
    {
        RenderCopy(const_cast<BEVector<BERenderable>::Ref>(FOREACH_VALUE));
    }
    RenderPresent();
}
