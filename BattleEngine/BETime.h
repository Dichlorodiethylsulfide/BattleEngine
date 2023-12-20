#pragma once
#include "BEString.h"

static constexpr size_t BETimeBufferSize = 33; // 32 + 1

struct BETime
{
    class BETimeContainer
    {
    public:
        BETimeContainer(long long CurrentTime)
            : m_currentTime(CurrentTime)
        {
        }
    const char* GetTimeBuffer();
    private:
        BEString m_currentTimeBuffer{BETimeBufferSize};
        long long m_currentTime{};
    };

    static BETimeContainer Now();
};
