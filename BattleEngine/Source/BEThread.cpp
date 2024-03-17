#include "BEThread.h"

void ThisThread::SleepFor(SizeType Milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
}
