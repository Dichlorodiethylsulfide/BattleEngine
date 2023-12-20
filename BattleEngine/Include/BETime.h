#pragma once
#include "BEString.h"
#include <ostream>

static constexpr size_t BETimeBufferSize = 33; // 32 + 1

struct BETime
{
    class BETimeContainer
    {
    public:
        BETimeContainer(long long CurrentTime);
    private:
        BEString m_currentTimeBuffer{BETimeBufferSize};
        long long m_currentTime{};
        friend std::ostream&
        operator<<(std::ostream& os, const BETimeContainer& container) {
            return os << "[" << container.m_currentTimeBuffer << "] ";
        }
    };

    static BETimeContainer Now();
};
