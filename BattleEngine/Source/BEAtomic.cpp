#include "BEAtomic.h"

#if !BE_USE_STD_ATOMICS

#if WINDOWS
#include <intrin0.h>
#include <xthreads.h>
#define InterlockedAdd8(Address, operand) (::_InterlockedExchangeAdd8((volatile char*)(Address), operand))
#define InterlockedAdd16(Address, operand) (::_InterlockedExchangeAdd16((volatile short*)(Address), operand))
#define InterlockedAdd32(Address, operand) (::_InterlockedExchangeAdd((volatile long*)(Address), operand))
#define InterlockedAdd64(Address, operand) (::_InterlockedExchangeAdd64((volatile long long*)(Address), operand))

#define InterlockedSub8(Address, operand) (::_InterlockedExchangeAdd8((volatile char*)(Address), operand))
#define InterlockedSub16(Address, operand) (::_InterlockedExchangeAdd16((volatile short*)(Address), operand))
#define InterlockedSub32(Address, operand) (::_InterlockedExchangeAdd((volatile long*)(Address), operand))
#define InterlockedSub64(Address, operand) (::_InterlockedExchangeAdd64((volatile long long*)(Address), operand))

#define InterlockedCompareExchange8(Address, target, value) (::_InterlockedCompareExchange8((char*)(Address), (target), (char)(value)))
#define InterlockedExchange8(Address, value) (::_InterlockedExchange8((char*)(Address), (char)(value)))

#define OSMutex _Mtx_t
#define MUTEX_INIT(mutex) _Mtx_init(mutex, _Mtx_plain)
#define MUTEX_DESTROY(mutex) _Mtx_destroy(mutex)
#define MUTEX_LOCK(mutex) _Mtx_lock(mutex)
#define MUTEX_UNLOCK(mutex) _Mtx_unlock(mutex)

#define OSCndVar _Cnd_t
#define CND_INIT(cnd) _Cnd_init(cnd)
#define CND_DESTROY(cnd) _Cnd_destroy(cnd)
#define CND_WAIT(cnd, mutex) _Cnd_wait(cnd, mutex)
#define CND_TIMED_WAIT(cnd, mutex, time) _Cnd_timedwait(cnd, mutex, time)
#define CND_SIGNAL(cnd) _Cnd_signal(cnd)
#define CND_BROADCAST(cnd) _Cnd_broadcast(cnd)

#define GET_THREAD_ID _Thrd_id()

#define THRD_SUCCESS _Thrd_success
#define THRD_TIMEDOUT _Thrd_timedout

#define TIMESPEC _timespec64
#else
#include <threads.h>
#include <threads.h>
// __atomic_* functions are built into GNU and so all modern versions of Linux should implement them under the __atomic and __sync libraries.
#define MEMORY_ORDER __ATOMIC_SEQ_CST
#define InterlockedAdd8(Address, operand) (__atomic_fetch_add(Address, operand, MEMORY_ORDER) + 1)
#define InterlockedAdd16(Address, operand) InterlockedAdd8(Address, operand)
#define InterlockedAdd32(Address, operand) InterlockedAdd8(Address, operand)
#define InterlockedAdd64(Address, operand) InterlockedAdd8(Address, operand)

#define InterlockedSub8(Address, operand) (__atomic_fetch_sub(Address, operand, MEMORY_ORDER) - 1)
#define InterlockedSub16(Address, operand) InterlockedSub8(Address, operand)
#define InterlockedSub32(Address, operand) InterlockedSub8(Address, operand)
#define InterlockedSub64(Address, operand) InterlockedSub8(Address, operand)

#define InterlockedCompareExchange8(Address, target, value) (__atomic_compare_exchange_n((char*)Address, (target), (char)(value), false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
#define InterlockedExchange8(Address, value) (__atomic_exchange_n((char*)Address, value, __ATOMIC_SEQ_CST))

#define OSMutex mtx_t
#define MUTEX_INIT(mutex) mtx_init(mutex, mtx_plain)
#define MUTEX_DESTROY(mutex) mtx_destroy(mutex)
#define MUTEX_LOCK(mutex) mtx_lock(mutex)
#define MUTEX_UNLOCK(mutex) mtx_unlock(mutex)

#define OSCndVar cnd_t
#define CND_INIT(cnd) cnd_init(cnd)
#define CND_DESTROY(cnd) cnd_destroy(cnd)
#define CND_WAIT(cnd, mutex) cnd_wait(cnd, mutex)
#define CND_TIMED_WAIT(cnd, mutex, time) cnd_timedwait(cnd, mutex, time)
#define CND_SIGNAL(cnd) cnd_signal(cnd)
#define CND_BROADCAST(cnd) cnd_broadcast(cnd)

#define GET_THREAD_ID (unsigned int)thrd_current()

#define THRD_SUCCESS thrd_success
#define THRD_TIMEDOUT thrd_timedout
#define TIMESPEC struct timespec
#endif

#define GET_OS_MUTEX(mutex) (static_cast<OSMutex*>(mutex))
#define GET_OS_INTERNAL_MUTEX(lock) (GET_OS_MUTEX((lock).m_Mutex->m_OSMutex))
#define GET_OS_CND(cnd) (static_cast<OSCndVar*>(cnd))

#define THROW_ERROR(x) PLATFORM_BREAK_
#define THROW_NOT_PERMITTED THROW_ERROR(TEXT("NOT_PERMITTED"))
#define THROW_DEADLOCK THROW_ERROR(TEXT("DEADLOCK"))

#define INTERLOCK_INTERNAL_FUNCTIONS(operation, bits) \
size_t AtomicInterlocked##operation##bits(AtomicAddress Address, size_t Operand) \
{ \
return Interlocked##operation##bits(Address, Operand); \
}

#define INTERLOCK_FUNCTION_BODY(operation, operand) \
static const operation##Function operation##Functions[9] = {nullptr, &AtomicInterlocked##operation##8, &AtomicInterlocked##operation##16, nullptr, &AtomicInterlocked##operation##32, nullptr, nullptr, nullptr, &AtomicInterlocked##operation##64}; \
return operation##Functions[Bytes](Address, operand)

#if WINDOWS
#pragma warning(push)
#pragma warning(disable:4267)

INTERLOCK_INTERNAL_FUNCTIONS(Add, 8)
INTERLOCK_INTERNAL_FUNCTIONS(Add, 16)
INTERLOCK_INTERNAL_FUNCTIONS(Add, 32)
INTERLOCK_INTERNAL_FUNCTIONS(Add, 64)

INTERLOCK_INTERNAL_FUNCTIONS(Sub, 8)
INTERLOCK_INTERNAL_FUNCTIONS(Sub, 16)
INTERLOCK_INTERNAL_FUNCTIONS(Sub, 32)
INTERLOCK_INTERNAL_FUNCTIONS(Sub, 64)
#pragma warning(pop)
#endif 

typedef SizeType (*AddFunction)(AtomicAddress, size_t);
typedef SizeType (*SubFunction)(AtomicAddress, size_t);

Int32 PRIVATE_NAMESPACE(BEAtomic)::GetThreadID()
{
    return GET_THREAD_ID;
}

size_t PRIVATE_NAMESPACE(BEAtomic)::AtomicInterlockedAdd(AtomicAddress Address, size_t Bytes, size_t Operand)
{
    INTERLOCK_FUNCTION_BODY(Add, Operand);
}

size_t PRIVATE_NAMESPACE(BEAtomic)::AtomicInterlockedSub(AtomicAddress Address, size_t Bytes, size_t Operand)
{
    INTERLOCK_FUNCTION_BODY(Add, -((ptrdiff_t)Operand));
}

bool PRIVATE_NAMESPACE(BEAtomic)::AtomicInterlockedCompare(AtomicAddress ContentsAddress, AtomicAddress ExpectedAddress, size_t Bytes)
{
    // TODO: malloc and free being used here from cstdlib
    // Very slow but very strong comparison
    // Enforcing this operation means the result is slower but it is guaranteed
    // because the contents are first copied byte-for-byte
    const auto& Allocator = GetAllocator();
    void* ContentBytes = Allocator.Malloc(Bytes);
    memcpy(ContentBytes, (void*)ContentsAddress, Bytes);
    void* ExpectedBytes = Allocator.Malloc(Bytes);
    memcpy(ExpectedBytes, (void*)ExpectedAddress, Bytes);
    char* ContentBytePtr = (char*)ContentBytes;
    char* ExpectedBytePtr = (char*)ExpectedBytes;
    for(size_t i = 0; i < Bytes; i++)
    {
        char DiscardByte = *(ContentBytePtr + i);
        char DiscardValue = DiscardByte != 1 ? 1 : 0;
#if WINDOWS
        InterlockedCompareExchange8(&DiscardByte, DiscardValue, *(ExpectedBytePtr + i));
#else
        // need to verify with the gnu docs
        //InterlockedCompareExchange8(&discardByte, (void*)&discardValue, *(expectedBytePtr + i));
        InterlockedCompareExchange8(&discardByte, (void*)(expectedBytePtr + i), discardValue);
#endif
        if(DiscardByte != DiscardValue)
        {
            Allocator.Free(ContentBytePtr);
            Allocator.Free(ExpectedBytePtr);
            return false;
        }
    }
    Allocator.Free(ContentBytePtr);
    Allocator.Free(ExpectedBytePtr);
    return true;
}

SMutex::SMutex()
{
    m_OSMutex = new OSMutex;
    MUTEX_INIT(GET_OS_MUTEX(m_OSMutex));
}

SMutex::~SMutex()
{
#if WINDOWS
    MUTEX_DESTROY(*GET_OS_MUTEX(m_OSMutex));
#else
    MUTEX_DESTROYGET_OS_MUTEX(m_OSMutex))
#endif
    delete GET_OS_MUTEX(m_OSMutex);
}

void SMutex::Lock() const
{
#if WINDOWS
    MUTEX_LOCK(*GET_OS_MUTEX(m_OSMutex));
#else
    MUTEX_LOCK(GET_OS_MUTEX(m_OSMutex));
#endif
}

void SMutex::Unlock() const
{
#if WINDOWS
    MUTEX_UNLOCK(*GET_OS_MUTEX(m_OSMutex));
#else
    MUTEX_UNLOCK(GET_OS_MUTEX(m_OSMutex));
#endif
}

void SUniqueLock::ValidateLock() const
{
    if(!m_Mutex)
    {
        THROW_NOT_PERMITTED
        return;
    }
    if(m_Owns)
    {
        THROW_DEADLOCK
        return;
    }
}

void SUniqueLock::ValidateUnlock() const
{
    if(!m_Mutex || !m_Owns)
    {
        THROW_NOT_PERMITTED
        return;
    }
}

SCondVar::SCondVar()
{
    m_OSCnd = new OSCndVar;
    CND_INIT(GET_OS_CND(m_OSCnd));
}

SCondVar::~SCondVar()
{
#if WINDOWS
    CND_DESTROY(*GET_OS_CND(m_OSCnd));
#else
    CND_DESTROY(GET_OS_CND(m_OSCnd));
#endif
}

void SCondVar::NotifyOne()
{
#if WINDOWS
    CND_SIGNAL(*GET_OS_CND(m_OSCnd));
#else
    CND_SIGNAL(GET_OS_CND(m_OSCnd));
#endif
}

void SCondVar::NotifyAll()
{
#if WINDOWS
    CND_BROADCAST(*GET_OS_CND(m_OSCnd));
#else
    CND_BROADCAST(GET_OS_CND(m_OSCnd));
#endif
}

void SCondVar::Wait(SUniqueLock& Lock)
{
#if WINDOWS
    CND_WAIT(*GET_OS_CND(m_OSCnd), *GET_OS_INTERNAL_MUTEX(Lock));
#else
    CND_WAIT(GET_OS_CND(m_OSCnd), GET_OS_INTERNAL_MUTEX(Lock));
#endif
}

ECndStatus SCondVar::WaitUntil(SUniqueLock& Lock, BETime::Milliseconds Duration)
{
    if(!Lock.OwnsLock())
    {
        THROW_NOT_PERMITTED
        return ECndStatus::Neither;
    }
    if(Duration.WaitTime == 0)
    {
        return ECndStatus::Timeout;
    }
    // TODO: check this conversion, we may be losing data
    TIMESPEC tspec = {static_cast<Int64>(BETime::BETimeContainer(Duration).As<BETime::Seconds>()), 0};
#if WINDOWS
    const int Result = CND_TIMED_WAIT(*GET_OS_CND(m_OSCnd), *GET_OS_INTERNAL_MUTEX(Lock), &tspec);
#else
    const int Result = CND_TIMED_WAIT(GET_OS_CND(m_OSCnd), GET_OS_INTERNAL_MUTEX(Lock), &tspec);
#endif
    switch (Result)
    {
    case THRD_SUCCESS:
        return ECndStatus::NoTimeout;
    case THRD_TIMEDOUT:
        return ECndStatus::Timeout;
    default:
        THROW_ERROR(Result);
    }
    return ECndStatus::Neither;
}

ECndStatus SCondVar::WaitFor(SUniqueLock& Lock, BETime::Milliseconds Duration)
{
    if(Duration.WaitTime > BETime::LargestTime)
    {
        return ECndStatus::NoTimeout;
    }
    return WaitUntil(Lock, Duration);
}

SSpinLockGuard::SSpinLockGuard(const SSpinLock& Lock)
{
#if WINDOWS
    // TODO: convert enum to underlying type
    while(!InterlockedCompareExchange8(&Lock, (char)EAtomicStateEnum::Locked, EAtomicStateEnum::Locked))
#else
    auto LockedEnum = EAtomicStateEnum::Locked;
    while(!InterlockedCompareExchange8(&Lock, (char*)&LockedEnum, EAtomicStateEnum::Locked))
#endif
    {
        // spin!
        // Blocking
    }
}

SSpinLockGuard::~SSpinLockGuard()
{
    InterlockedExchange8(&m_SpinLock, EAtomicStateEnum::Unlocked);
}

#endif