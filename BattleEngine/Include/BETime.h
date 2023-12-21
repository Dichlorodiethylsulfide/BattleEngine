#pragma once
#include "BEString.h"
#include <ostream>

static constexpr size_t BETimeBufferSize = 33; // 32 + 1

struct BETime
{
    using SizeType = long long;
    
    template<SizeType TTime>
    struct Duration
    {
        static constexpr SizeType Rep = TTime;
    };

    using Nanoseconds = Duration<1>;
    using Microseconds = Duration<Nanoseconds::Rep * 1000>;
    using Milliseconds = Duration<Microseconds::Rep * 1000>;
    using Seconds = Duration<Milliseconds::Rep * 1000>;
    
    class BETimeContainer
    {
    public:
        BETimeContainer(SizeType CurrentTime);
        BETimeContainer(const BETimeContainer&) = default;
        SizeType GetTime() const
        {
            return m_currentTime;
        }
        template<typename TimeRep>
        SizeType As() const
        {
            return m_currentTime / TimeRep::Rep;
        }
    private:
        SizeType m_currentTime{};
    };
    
    class BEDisplayableTime : public BETimeContainer
    {
    public:
        BEDisplayableTime(SizeType CurrentTime);
    private:
        BEString m_currentTimeBuffer{BETimeBufferSize};
        friend std::ostream&
        operator<<(std::ostream& os, const BEDisplayableTime& container) {
            return os << "[" << container.m_currentTimeBuffer << "] ";
        }
    };

    class BETimeKeeper : public BETimeContainer
    {
    public:
        BETimeKeeper(SizeType CurrentTime)
            : BETimeContainer(CurrentTime)
        {
            m_lastCallTime = CurrentTime;
        }
        BETimeKeeper(const BETimeContainer& Container)
            : BETimeContainer(Container)
        {
        }
        BETimeKeeper(const BETimeKeeper&) = default;
        BETimeKeeper SinceLastCall();
    private:
        SizeType m_lastCallTime{};
    };
    
    static BEDisplayableTime NowAsString(); // uses system_clock
    static BETimeContainer Now(); // uses high_resolution_clock
    static void WaitForMilliseconds(SizeType Milliseconds);
};
