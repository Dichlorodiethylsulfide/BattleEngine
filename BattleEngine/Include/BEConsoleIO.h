#pragma once

#include "BEString.h"

enum BEIOColourCode
{
    // Foreground
    BE_FG_RED      = 31,
    BE_FG_GREEN    = 32,
    BE_FG_YELLOW   = 33,
    BE_FG_BLUE     = 34,
    BE_FG_WHITE    = 37,
    BE_FG_DEFAULT  = 39,
    // Background
    BE_BG_RED      = 41,
    BE_BG_GREEN    = 42,
    BE_BG_YELLOW   = 43,
    BE_BG_BLUE     = 44,
    BE_BG_WHITE    = 47,
    BE_BG_DEFAULT  = 49
};

enum class BEIOReportMode : int
{
    None,
    Log,
    Debug,
    Success,
    Warning,
    Error,
    All
};

struct BEIOReporterSpecifier
{
    BEIOColourCode Code;
    BEIOReportMode Mode;
    const char* BEReportString;
};

#define DEFINE_REPORTER_SPECIFIER(name, color) \
    static constexpr BEIOReporterSpecifier name##Reporter = {color, BEIOReportMode::##name, #name};

DEFINE_REPORTER_SPECIFIER(Log, BE_FG_DEFAULT);
DEFINE_REPORTER_SPECIFIER(Debug, BE_FG_WHITE);
DEFINE_REPORTER_SPECIFIER(Warning, BE_FG_YELLOW);
DEFINE_REPORTER_SPECIFIER(Error, BE_FG_RED);
DEFINE_REPORTER_SPECIFIER(Success, BE_FG_GREEN);

#define DEFINE_LOG_OUTPUT(name) \
template<typename TElem> \
void name##Output(TElem BEValue) \
{ \
    if constexpr((std::is_integral_v<TElem> || std::is_floating_point_v<TElem>) && !std::is_same_v<::BEString::ValueType, TElem>) \
    {\
        auto BEString = ::BEString::ToString(BEValue);\
        BEConsoleIO::Get().Output(BEString, name##Reporter); \
    }\
    else \
    {   auto BEString = ::BEString(BEValue); \
        BEConsoleIO::Get().Output(BEString, name##Reporter); \
    } \
} \
template<typename TElem> \
void name##OutputClean(TElem BEValue) \
{ \
    if constexpr((std::is_integral_v<TElem> || std::is_floating_point_v<TElem>) && !std::is_same_v<::BEString::ValueType, TElem>) \
    {\
        auto BEString = ::BEString::ToString(BEValue);\
        BEConsoleIO::Get().OutputClean(BEString, name##Reporter); \
    }\
    else \
    {   auto BEString = ::BEString(BEValue); \
        BEConsoleIO::Get().OutputClean(BEString, name##Reporter); \
    } \
} 

class BEConsoleIO
{
    BEConsoleIO() {}
    BEConsoleIO(const BEConsoleIO&) = delete;
    BEConsoleIO(BEConsoleIO&&) = delete;
    ~BEConsoleIO() {}
    
    void Output(BEString& BEString, const BEIOReporterSpecifier& Reporter);
    void OutputClean(BEString& BEString, const BEIOReporterSpecifier& Reporter);
    void OutputLineBreak();
public:
    static BEConsoleIO& Get()
    {
        static BEConsoleIO IORepoter;
        return IORepoter;
    }
    static void EnableLoggingForAllLevels()
    {
        Get().ModesUnderDisabled = BEIOReportMode::None;
    }
    // But not including this level
    static void DisableLoggingUnderLevel(BEIOReportMode Mode)
    {
        Get().ModesUnderDisabled = Mode;
    }
    static void DisableAllLogging()
    {
        Get().ModesUnderDisabled = BEIOReportMode::All;
    }
    static void LineBreak()
    {
        Get().OutputLineBreak();
    }
    DEFINE_LOG_OUTPUT(Log)
    DEFINE_LOG_OUTPUT(Debug)
    DEFINE_LOG_OUTPUT(Warning)
    DEFINE_LOG_OUTPUT(Error)
    DEFINE_LOG_OUTPUT(Success)

private:
    BEIOReportMode ModesUnderDisabled = BEIOReportMode::None;
};

#define CALL_REPORTER(name, x) BEConsoleIO::Get().##name##Output(x)
#define CALL_REPORTER_CLEAN(name, x) BEConsoleIO::Get().##name##OutputClean(x)

#if _DEBUG
#define DEBUG(x) CALL_REPORTER(Debug, x)
#else
#define DEBUG(x)
#endif

#define LOG(x) CALL_REPORTER(Log, x)
#define WARN(x) CALL_REPORTER(Warning, x)
#define ERROR(x) do { \
    CALL_REPORTER(Error, x); \
    CALL_REPORTER_CLEAN(Error, __FILE__); \
    CALL_REPORTER_CLEAN(Error, __LINE__); \
    BEConsoleIO::LineBreak(); } \
    while(0)
#define SUCCESS(x) CALL_REPORTER(Success, x)

/*template<typename FirstArgument, typename ... Arguments>
void nibc_print(FirstArgument&& FirstArgs, Arguments&&... RestArgs)
{
    nibc_print(std::forward<FirstArgument>(FirstArgs), false);
    nibc_print(std::forward<Arguments>(RestArgs)...);
}*/