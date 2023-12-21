#include "BETickable.h"

BETickable::BETickable()
{
    m_tickState = BETickState::Initialised;
}

BETickable::~BETickable()
{
}

void BETickable::InitialTick()
{
    m_tickState = BETickState::Running;
}

void BETickable::Tick()
{
}

void BETickable::FinalTick()
{
    if(m_tickState != BETickState::Stopped)
    {
        m_tickState = BETickState::Finished;
    }
}
