#include "BEConsoleIO.h"
#include "BERenderer.h"

#include "SDL.h"
#include "SDL_image.h"


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
