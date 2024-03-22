#pragma once

#include "BETypeTraits.h"

#if BE_USE_STD_THREADS
#include <thread>
using SThread = std::thread;
#else
#endif

namespace ThisThread
{
    Int32 GetThreadID();
    void SleepFor(SizeType Nanoseconds);
    // void SleepUntil(SizeType Nanoseconds); // TODO: need to calculate time offsets and potentially predicates?
    void Yield(); // TODO: coroutine yield?
}

