#pragma once

#include "BETypeTraits.h"

#if BE_USE_STD_THREADS
#include <thread>
using SThread = std::thread;
#else
#endif

namespace ThisThread
{
    void SleepFor(SizeType Milliseconds);
}
