#include "BETime.h"
#include "BEThread.h"

BETime::BETimeContainer::BETimeContainer(long long CurrentTime)
    : m_currentTime(CurrentTime)
{
}

BETime::BEDisplayableTime::BEDisplayableTime(long long CurrentTime)
    : BETimeContainer(CurrentTime)
{
    if(ctime_s(m_currentTimeBuffer.CBuffer(), BETimeBufferSize, &CurrentTime)) // > 0 is an error
    {
        m_currentTimeBuffer = "Error converting ctime to BETime"; // 32 characters -- do not modify
    }
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
    ThisThread::SleepFor(Milliseconds);
}