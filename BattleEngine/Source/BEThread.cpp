#include "BEThread.h"

void ThisThread::SleepFor(SizeType Nanoseconds)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(Nanoseconds));
}
