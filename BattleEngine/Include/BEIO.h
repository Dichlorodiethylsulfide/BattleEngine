#pragma once

#include "BEString.h"

enum BEIOColourCode
{
    BE_FG_RED      = 31,
    BE_FG_GREEN    = 32,
    BE_FG_YELLOW   = 33,
    BE_FG_BLUE     = 34,
    BE_FG_DEFAULT  = 39,
    BE_BG_RED      = 41,
    BE_BG_GREEN    = 42,
    BE_BG_YELLOW   = 43,
    BE_BG_BLUE     = 44,
    BE_BG_DEFAULT  = 49
};

enum class BEIOReportMode
{
    Debug,
    Warning,
    Error
};

struct BEIOReporterSpecifier
{
    BEIOColourCode Code;
    BEIOReportMode Mode;
    const char* BEReportString;
};

#define DEFINE_REPORTER_SPECIFIER(name, color, mode) \
    static constexpr BEIOReporterSpecifier name##Reporter = {color, mode, #mode};

DEFINE_REPORTER_SPECIFIER(Debug, BE_FG_DEFAULT, BEIOReportMode::Debug);
DEFINE_REPORTER_SPECIFIER(Warn, BE_FG_YELLOW, BEIOReportMode::Warning);
DEFINE_REPORTER_SPECIFIER(Error, BE_FG_RED, BEIOReportMode::Error);

#define DEFINE_LOG_OUTPUT(name) \
static void name##Output(BEString BEString) \
{ \
    BEIO::Output(BEString, name##Reporter); \
}

class BEIO
{
    static void Output(BEString& BEString, const BEIOReporterSpecifier& Reporter);
public:
    DEFINE_LOG_OUTPUT(Debug)
    DEFINE_LOG_OUTPUT(Warn)
    DEFINE_LOG_OUTPUT(Error)
};

#if _DEBUG
#define DEBUG(x) BEIO::DebugOutput(x)
#else
#define DEBUG(x)
#endif

#define WARN(x) BEIO::WarnOutput(x)
#define ERROR(x) BEIO::ErrorOutput(x)