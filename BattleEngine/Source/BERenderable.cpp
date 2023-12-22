#include "BEConsoleIO.h"
#include "BERenderer.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_render.h"

BERenderable::~BERenderable()
{
    if(m_renderTexture)
    {
        SDL_DestroyTexture(m_renderTexture);
    }
}

void BERenderable::LoadTexture(BERenderer& Renderer, BEString TextureFilepath, BEBox<int> Rect)
{
    if(!Renderer)
    {
        ERROR("Invalid Renderer Pointer passed through renderable.");
        return;
    }
    m_renderTexture = IMG_LoadTexture(Renderer.m_rendererPointer, TextureFilepath);
    m_rect = Rect;
}
