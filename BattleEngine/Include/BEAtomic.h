#pragma once

#include "BETime.h"
#include "BETypeTraits.h"

// Clear macro to test atomic code
#if defined(BE_USE_STD_ATOMICS)
#undef BE_USE_STD_ATOMICS
#define BE_USE_STD_ATOMICS 0
#else
#define BE_USE_STD_ATOMICS 0
#endif

#if BE_USE_STD_ATOMICS
#include <atomic>
#include <mutex>
template<typename T>
using TAtomic = std::atomic<T>;
using SMutex = std::mutex;
using SCondVar = std::condition_variable;
#else

using AtomicAddress = VOLATILE Int32*;
using AtomicRef = VOLATILE Int32&;

PRIVATE_NAMESPACE_DEFINE(BEAtomic)
{
    Int32 GetThreadID();
    SizeType AtomicInterlockedAdd(AtomicAddress Address, SizeType Bytes, SizeType Operand);
    SizeType AtomicInterlockedSub(AtomicAddress Address, SizeType Bytes, SizeType Operand);
    bool AtomicInterlockedCompare(AtomicAddress ContentsAddress, AtomicAddress ExpectedAddress, SizeType Bytes);

    template<class TType>
    BE_FORCEINLINE AtomicAddress GetAtomicAddress(TType& Source) noexcept
    {
        return &reinterpret_cast<AtomicRef>(Source);
    }

    template<class TType>
    BE_FORCEINLINE AtomicAddress GetAtomicAddress(const TType& Source) noexcept
    {
        return &reinterpret_cast<const AtomicRef>(Source);
    }
    
    template<class TType>
    BE_FORCEINLINE bool AtomicCompare(VOLATILE TType* ContentsAddress, VOLATILE TType* ExpectedAddress)
    {
        return AtomicInterlockedCompare(GetAtomicAddress(ContentsAddress), GetAtomicAddress(ExpectedAddress), sizeof(TType));
    }
} // BEAtomic_Private

enum class EAtomicStateEnum : UInt8
{
    Locked = 0,
    Unlocked
};

class SSpinLock
{
    EAtomicStateEnum LockEnum = EAtomicStateEnum::Unlocked;
};

class SMutex
{
public:
    SMutex();
    ~SMutex();

    BE_DISABLE_ASSIGNMENT(SMutex)
    BE_DISABLE_CONSTRUCTION(SMutex)

    void Lock() const;
    void Unlock() const;

private:
    friend class SCondVar;
    void* m_OSMutex;
};

class SUniqueLock
{
public:
    SUniqueLock() = delete;
    ~SUniqueLock() { Unlock(); } // TODO: check this
    BE_DISABLE_ASSIGNMENT(SUniqueLock)
    BE_DISABLE_CONSTRUCTION(SUniqueLock)

    explicit SUniqueLock(SMutex& Mutex)
    {
        m_Mutex = &Mutex;
    }

    void Lock()
    {
        ValidateLock();
        m_Mutex->Lock();
        m_Owns = true;
    }

    void Unlock()
    {
        ValidateUnlock();
        m_Mutex->Unlock();
        m_Owns = false;
    }

    bool OwnsLock() const
    {
        return m_Owns;
    }
private:
    friend class SCondVar;
    void ValidateLock() const;
    void ValidateUnlock() const;
    bool m_Owns = false;
    SMutex* m_Mutex = nullptr;
};

enum class ECndStatus
{
    NoTimeout,
    Timeout,
    Neither
};

class SCondVar
{
public:
    SCondVar();
    ~SCondVar();

    BE_DISABLE_ASSIGNMENT(SCondVar)
    BE_DISABLE_CONSTRUCTION(SCondVar)

    void NotifyOne();
    void NotifyAll();
    void Wait(SUniqueLock& Lock);

    template<typename Predicate>
    void Wait(SUniqueLock& Lock, Predicate Pred) // replace with UniqueFunction ?
    {
        while(!Pred())
        {
            Wait(Lock);
        }
    }

    ECndStatus WaitUntil(SUniqueLock& Lock, BETime::Milliseconds Duration);
    ECndStatus WaitFor(SUniqueLock& Lock, BETime::Milliseconds Duration);

private:
    void* m_OSCnd;
};

class SSpinLockGuard
{
public:
    SSpinLockGuard() = delete;
    SSpinLockGuard(SSpinLockGuard&&) = delete;
    BE_DISABLE_ASSIGNMENT(SSpinLockGuard)
    
    explicit SSpinLockGuard(const SSpinLock&);
    ~SSpinLockGuard();
private:
    SSpinLock m_SpinLock;
};

class SCriticalAtomicSection
{
public:
    SCriticalAtomicSection() = default;

    virtual ~SCriticalAtomicSection()
    {
        SSpinLockGuard Guard { m_SpinLock };
        {
            if(IsInCriticalSection())
            {
                ExitCriticalSection();
            }
        }
    }

    void EnterCriticalSection()
    {
        SSpinLockGuard Guard { m_SpinLock };
        {
            CriticalThreadHandle = BEAtomic_Private::GetThreadID();
            bIsInCriticalSection = true;
        }
    }

    void ExitCriticalSection()
    {
        SSpinLockGuard Guard { m_SpinLock };
        {
            CriticalThreadHandle = 0;
            bIsInCriticalSection = false;
        }
    }

    bool IsInCriticalSection() const
    {
        SSpinLockGuard Guard { m_SpinLock };
        {
            return bIsInCriticalSection;
        }
    }

    bool IsThisThreadCritical() const
    {
        return IsThreadCritical(BEAtomic_Private::GetThreadID());
    }

    bool IsThreadCritical(Int32 ThreadID) const
    {
        SSpinLockGuard Guard { m_SpinLock };
        {
            return bIsInCriticalSection ? CriticalThreadHandle == ThreadID : true;
        }
    }
    
protected:
    SSpinLock m_SpinLock = SSpinLock();
private:
    bool bIsInCriticalSection = false;
    Int32 CriticalThreadHandle = 0;
};

template<class TType>
class SAtomicStorage : public SCriticalAtomicSection
{
public:
    using TValue = TRemoveReferenceT<TType>;

    SAtomicStorage()
    {
        SAtomicStorage::Clear();
    }

    SAtomicStorage(const TValue Value) noexcept
    {
        SAtomicStorage::Store(Value);
    }

    ~SAtomicStorage() override
    {
        SAtomicStorage::Clear();
    }

    TValue operator=(const TValue Value)
    {
        SAtomicStorage::Store(Value);
        return *this;
    }

    virtual TValue Store(TValue Value)
    {
        if(IsThisThreadCritical())
        {
            SSpinLockGuard Guard { m_SpinLock };
            {
                TValue Local = m_LockedValue;
                m_LockedValue = Value;
                return Local;
            }
        }
        return {};
    }

    virtual TValue Load() const
    {
        if(IsThisThreadCritical())
        {
            SSpinLockGuard Guard { m_SpinLock };
            {
                TValue Local = m_LockedValue;
                return Local;
            }
        }
        return {};
    }

    TValue Clear()
    {
        if(IsThisThreadCritical())
        {
            SSpinLockGuard Guard { m_SpinLock };
            {
                TValue Local = m_LockedValue;
                m_LockedValue = {};
                return Local;
            }
        }
        return {};
    }

    bool CompareWeak(TValue Value) const
    {
        return Load() == Value;
    }

    bool CompareStrong(TValue Value) const
    {
        TValue Local = Load();
        SSpinLockGuard Guard { m_SpinLock };
        {
            return BEAtomic_Private::AtomicCompare(&Local, &Value);
        }
    }

    virtual operator TType() const
    {
        return m_LockedValue;
    }

protected:
    TType m_LockedValue;
};

template<class TType>
class SAtomicFacade : public SAtomicStorage<TType>
{
public:
    using Base = SAtomicStorage<TType>;
    using TValue = typename Base::TValue;

    SAtomicFacade() : Base() {}
    SAtomicFacade(TValue Value) : Base(Value) {}
};

template<class TType>
class SAtomicIntegral final : public SAtomicFacade<TType>
{
    BE_T_ASSERT("Integral type only, except bool", TIsArithmeticV<TType> && !TIsSameV<TType, bool>)
public:
    using Base = SAtomicFacade<TType>;
    using TValue = typename Base::TValue;

    SAtomicIntegral() : Base({}) {}
    SAtomicIntegral(TValue Value) : Base(Value) {}

    TValue operator-=(TValue Value)
    {
        if(Base::IsThisThreadCritical())
        {
            return BEAtomic_Private::AtomicInterlockedSub(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TType), Value);
        }
        return {};
    }

    TValue operator+=(TValue Value)
    {
        if(Base::IsThisThreadCritical())
        {
            return BEAtomic_Private::AtomicInterlockedAdd(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TType), Value);
        }
        return {};
    }

    TValue operator++()
    {
        if(Base::IsThisThreadCritical())
        {
            return BEAtomic_Private::AtomicInterlockedAdd(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TType), 1);
        }
        return {};
    }

    TValue operator++(int)
    {
        if(Base::IsThisThreadCritical())
        {
            TValue Local = BEAtomic_Private::AtomicInterlockedAdd(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TType), 1);
            --Local;
            return Local;
        }
        return {};
    }

    TValue operator--()
    {
        if(Base::IsThisThreadCritical())
        {
            return BEAtomic_Private::AtomicInterlockedSub(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TType), 1);
        }
        return {};
    }

    TValue operator--(int)
    {
        if(Base::IsThisThreadCritical())
        {
            TValue Local = BEAtomic_Private::AtomicInterlockedSub(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TType), 1);
            ++Local;
            return Local;
        }
        return {};
    }
};

class SAtomicBool final : public SAtomicFacade<bool>
{
public:
    using Base = SAtomicFacade;
    using TValue = typename Base::TValue;

    SAtomicBool() : Base(false) {}
    SAtomicBool(bool Value) : Base(Value) {}

    bool Invert()
    {
        if(Base::IsThisThreadCritical())
        {
            TValue Local = Load();
            Store(!Local);
            return Local;
        }
        return false;
    }

    explicit operator bool() const
    {
        return Load();
    }
};

template<class TType>
class SAtomicPointer : public SAtomicFacade<TAddPointerT<TType>>
{
public:
    using Base = SAtomicFacade<TAddPointerT<TType>>;
    using TValuePtr = typename Base::TValue;
    using TValue = TType;

    SAtomicPointer() : Base(nullptr) {}
    SAtomicPointer(TValuePtr Value) : Base(Value) {}

    // TODO: FetchAdd and FetchSub should be implemented on integrals too?
    TValuePtr FetchAdd(SizeType Operand)
    {
        if(Base::IsThisThreadCritical())
        {
            // TODO: check this
            return BEAtomic_Private::AtomicInterlockedAdd(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TValuePtr), Operand);
        }
        return {};
    }
    
    TValuePtr FetchSub(SizeType Operand)
    {
        if(Base::IsThisThreadCritical())
        {
            // TODO: check this
            return BEAtomic_Private::AtomicInterlockedSub(BEAtomic_Private::GetAtomicAddress(Base::m_LockedValue), sizeof(TValuePtr), Operand);
        }
        return {};
    }

    TValuePtr operator++()
    {
        return FetchAdd(1);
    }
        
    TValuePtr operator++(int)
    {
        TValue _Local = FetchAdd(1);
        --_Local;
        return _Local;
    }

    TValuePtr operator--()
    {
        return FetchSub(1);
    }
        
    TValuePtr operator--(int)
    {
        TValue _Local = FetchSub(1);
        ++_Local;
        return _Local;
    }
};

template<class TType>
using TAtomic = typename TChooseType<TIsBoolV<TType>, SAtomicBool,
                typename TChooseType<TIsArithmeticV<TType>, SAtomicIntegral<TType>,
                typename TChooseType<TIsPointerV<TType>, SAtomicPointer<TType>,
                SAtomicFacade<TType>>::Type>::Type>::Type;

#endif
