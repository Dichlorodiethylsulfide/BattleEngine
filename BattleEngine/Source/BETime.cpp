#include "BETime.h"

#include<ctime>
#include<chrono>

BETime::BETimeContainer::BETimeContainer(long long CurrentTime)
    : m_currentTime(CurrentTime)
{
    if(const auto error = ctime_s(m_currentTimeBuffer.GetBuffer(), BETimeBufferSize, &m_currentTime); !error)
    {
        m_currentTimeBuffer.Strip();
    }
    else
    {
        m_currentTimeBuffer = "Error";
    }
}

BETime::BETimeContainer BETime::Now()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
