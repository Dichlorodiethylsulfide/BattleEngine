#include "BEThread.h"

#if WINDOWS
#define GET_THREAD_ID _Thrd_id()
#define YIELD _Thrd_yield()
#else
#define GET_THREAD_ID (unsigned int)thrd_current()
#defne YIELD  thrd_yield();
#endif

Int32 ThisThread::GetThreadID()
{
    return GET_THREAD_ID;
}

void ThisThread::SleepFor(SizeType Nanoseconds)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(Nanoseconds));
}

void ThisThread::Yield()
{
    YIELD;
}
