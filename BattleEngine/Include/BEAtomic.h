#pragma once

#include "BETypeTraits.h"

#define BE_USE_STD_ATOMICS 1 // my intention is to implement my own but use this for now

#if BE_USE_STD_ATOMICS
#include <atomic>
#include <mutex>
template<typename T>
using TAtomic = std::atomic<T>;
using SMutex = std::mutex;
using SCondVar = std::condition_variable;
#else
#endif