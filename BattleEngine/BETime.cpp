#include "BETime.h"

#include<ctime>
#include<chrono>

const char* BETime::BETimeContainer::GetTimeBuffer()
{
    if(const auto error = ctime_s(m_currentTimeBuffer.GetBuffer(), BETimeBufferSize, &m_currentTime); !error)
    {
        return m_currentTimeBuffer.GetBuffer();
    }
    return "";
}

BETime::BETimeContainer BETime::Now()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
