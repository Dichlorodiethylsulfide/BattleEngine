#pragma once

#include "BETickable.h"

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
};

class BEGPULoop : public BEEnineLoop
{
public:
    void EngineTick() override;
};
