#pragma once
#include "BEString.h"
#include "BEStream.h"

static constexpr size_t BETimeBufferSize = 33; // 32 + 1

struct BETime
{
    BE_FORCEINLINE static errno_t GetCTime(Char* const Buffer, SizeType const SizeInWords, time_t const* const Time);

    /*
     * Describes an immutable amount of time
     * Time calculations should be placed in BETimeContainer
     */
    template<SizeType TCount, typename TTime = SizeType> struct Duration;
    template<SizeType TCount>
    struct Duration<TCount>
    {
        static constexpr SizeType Rep = TCount;
        const SizeType WaitTime = 0;
        Duration(SizeType Count)
        {
            BE_MODIFY_CONST(WaitTime, Count * Rep);
        }
    };
    template<SizeType TCount, typename TTime>
    struct Duration
    {
        static constexpr SizeType WaitTime = TTime::Rep * TCount;
    };

    using Nanoseconds = Duration<1>;
    using Microseconds = Duration<Nanoseconds::Rep * 1000>;
    using Milliseconds = Duration<Microseconds::Rep * 1000>;
    using Seconds = Duration<Milliseconds::Rep * 1000>;
    using Minutes = Duration<Seconds::Rep * 60>;
    using Hours = Duration<Minutes::Rep * 60>;
    using Days = Duration<Hours::Rep * 24>;

    static constexpr SizeType LargestTime = Duration<7, Days>::WaitTime;

    /*
     * Describes an amount of time that can be updated, changed, etc
     */
    class BETimeContainer
    {
    public:
        BETimeContainer(Int64 CurrentTime);
        
        template<SizeType TTime>
        BETimeContainer(Duration<TTime> CurrentTime)
            : m_currentTime(CurrentTime.WaitTime)
        {
        }
        
        BETimeContainer(const BETimeContainer&) = default;
        BETimeContainer(BETimeContainer&&) = default;
        
        SizeType GetTime() const
        {
            return m_currentTime;
        }
        template<typename TimeRep>
        SizeType As() const
        {
            return m_currentTime / TimeRep::Rep;
        }
        BETimeContainer operator-(const BETimeContainer& Other) const
        {
            return m_currentTime - Other.m_currentTime;
        }
        BETimeContainer operator+(const BETimeContainer& Other) const
        {
            return m_currentTime - Other.m_currentTime;
        }
    private:
        Int64 m_currentTime{};
    };
    
    class BEDisplayableTime : public BETimeContainer
    {
    public:
        BEDisplayableTime(Int64 CurrentTime);
    private:
        BEString m_currentTimeBuffer{BETimeBufferSize};
        friend SOStream&
        operator<<(SOStream& os, const BEDisplayableTime& container) {
            return os << "[" << container.m_currentTimeBuffer.CStr() << "] ";
        }
    };
    
    static BEDisplayableTime NowAsString(); // uses system_clock
    static BETimeContainer Now(); // uses high_resolution_clock

    template<SizeType Nano>
    static void WaitFor(const Duration<Nano> Time)
    {
        WaitForNanoseconds(Time.WaitTime);
    }
    
    static void WaitForNanoseconds(SizeType Nanoseconds);
};
