#pragma once

#include "BETypeTraits.h"

#if BE_USE_STD_ATOMICS
#include <atomic>
#include <mutex>
template<typename T>
using TAtomic = std::atomic<T>;
using SMutex = std::mutex;
using SCondVar = std::condition_variable;
#else
#endif