#include "BEConsoleIO.h"
#include "BERenderer.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


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
    if(m_renderTexture)
    {
        ERROR("Failed to load texture from FilePath.");
        return;
    }
    m_rect = Rect;
}
