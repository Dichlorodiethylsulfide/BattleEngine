#pragma once

enum class BETickState
{
    Uninitialised, // Pre-Constructor
    Initialised, // Post-Constructor but before First Tick
    Stopped, // Stopped Prematurely, will not resume
    Paused, // Paused due to some conditions but will resume
    Running, // Currently ticking
    Finished // Post-Ticking but Pre-Destructor
};

class BETickable
{
public:
    BETickable();
    virtual ~BETickable();
    virtual void InitialTick(); // First Tick after construction
    virtual void Tick();
    virtual void FinalTick(); // Last tick before destruction
private:
    BETickState m_tickState = BETickState::Uninitialised;
};
