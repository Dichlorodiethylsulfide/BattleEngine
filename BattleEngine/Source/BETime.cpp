#include "BETime.h"

#include<ctime>
#include<chrono>

BETime::BETimeContainer::BETimeContainer(long long CurrentTime)
    : m_currentTime(CurrentTime)
{
}

BETime::BEDisplayableTime::BEDisplayableTime(long long CurrentTime)
    : BETimeContainer(CurrentTime)
{
    if(const auto error = ctime_s(m_currentTimeBuffer.GetBuffer(), BETimeBufferSize, &CurrentTime); !error)
    {
        m_currentTimeBuffer.Strip();
    }
    else
    {
        m_currentTimeBuffer = "Error converting 'long long' Time to 'BEString' Time";
    }
}

BETime::BETimeKeeper BETime::BETimeKeeper::SinceLastCall()
{
    const auto lastCallTime = m_lastCallTime;
    m_lastCallTime = Now().GetTime();
    return m_lastCallTime - lastCallTime;
}

BETime::BEDisplayableTime BETime::NowAsString()
{
    return {std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
}

BETime::BETimeContainer BETime::Now()
{
    using namespace std::chrono;
    return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

void BETime::WaitForMilliseconds(long long Milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
}
