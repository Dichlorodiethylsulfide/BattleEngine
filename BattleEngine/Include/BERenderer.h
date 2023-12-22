#pragma once

#include "BERenderable.h"
#include "BEUtilities.h"

class BERenderer
{
public:
    BERenderer() = default;
    BERenderer(External WindowPointer);
    ~BERenderer();
    operator bool() const
    {
        return m_rendererPointer;
    }
    void ClearWindow();
    void RenderCopy(BERenderable& Renderable);
    void RenderPresent();
protected:
    friend class BERenderable;
    External m_rendererPointer{};
};
