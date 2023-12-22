#pragma once
#include "BEString.h"
#include "BEUtilities.h"

class BERenderable
{
public:
    BERenderable() = default;
    ~BERenderable();
    void LoadTexture(class BERenderer& Renderer, BEString TextureFilepath, BEBox<int> Rect);
private:
    friend class BERenderer;
    External m_renderTexture{};
    BEBox<int> m_rect{};
};
