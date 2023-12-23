#include "BERenderer.h"
#include "BEConsoleIO.h"

#include "SDL_render.h"

BERenderer::BERenderer(External WindowPointer)
{
    if(!WindowPointer)
    {
        ERROR("Invalid Window Pointer passed through to Renderer.");
        return;
    }
    m_rendererPointer = SDL_CreateRenderer(WindowPointer, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!m_rendererPointer)
    {
        ERROR("Failed to create Rendering Context.");
        return;
    }
}

BERenderer::~BERenderer()
{
    if(m_rendererPointer)
    {
        SDL_DestroyRenderer(m_rendererPointer);
    }
}

void BERenderer::ClearWindow()
{
    if(m_rendererPointer)
    {
        SDL_ClearError();
        if(SDL_RenderClear(m_rendererPointer) < 0)
        {
            ERROR(SDL_GetError());
        }
    }
}

void BERenderer::RenderCopy(BERenderable& Renderable)
{
    if(m_rendererPointer)
    {
        SDL_ClearError();
        const SDL_Rect rect = {Renderable.m_rect.TopLeft.X, Renderable.m_rect.TopLeft.Y, Renderable.m_rect.Dimensions.X, Renderable.m_rect.Dimensions.Y};
        if(SDL_RenderCopy(m_rendererPointer, Renderable.m_renderTexture, nullptr, &rect) < 0)
        {
            ERROR(SDL_GetError());
        }
    }
}

void BERenderer::RenderPresent()
{
    SDL_RenderPresent(m_rendererPointer);
}
