#pragma once

#include "BEVector.h"
#include "BETickable.h"
#include "BERenderer.h"

class BEEnineLoop
{
public:
    virtual ~BEEnineLoop() {}
    virtual void EngineTick() = 0;
    bool inline IsTicking(BETickState TickState) const
    {
        return m_tickState == TickState;
    }
protected:
    BETickState m_tickState = BETickState::Running;
};

class BECPULoop : public BEEnineLoop// main engine loop, performed on the CPU, might need a GPU loop for GPU operations such as rendering
{
public:
    void EngineTick() override;
private:
    BEVector<BETickable> m_tickableObjects{};
};

class BEGPULoop : public BEEnineLoop, public BERenderer
{
public:
    BEGPULoop() = default;
    BEGPULoop(External WindowPointer);
    void EngineTick() override;
private:
    BEVector<BERenderable> m_renderableObjects{};
};
