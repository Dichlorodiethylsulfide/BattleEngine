#include "BETime.h"
#include "BEThread.h"

errno_t BETime::GetCTime(Char* const Buffer, SizeType const SizeInWords, time_t const* const Time)
{
#if UNICODE
    return _wctime_s(Buffer, SizeInWords, Time);
#else
    return ctime_s(Buffer, SizeInWords, Time);
#endif
}

BETime::BETimeContainer::BETimeContainer(Int64 CurrentTime)
    : m_currentTime(CurrentTime)
{
}

BETime::BEDisplayableTime::BEDisplayableTime(Int64 CurrentTime)
    : BETimeContainer(CurrentTime)
{
    if(GetCTime(m_currentTimeBuffer.CBuffer(), BETimeBufferSize, &CurrentTime)) // > 0 is an error
    {
        m_currentTimeBuffer = TEXT("Error converting ctime to BETime"); // 32 characters -- do not modify
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

void BETime::WaitForNanoseconds(SizeType Nanoseconds)
{
    if(Nanoseconds > LargestTime)
    {
        PLATFORM_BREAK("Number of nanoseconds is larger than 7 days")
    }
    ThisThread::SleepFor(Nanoseconds);
}
