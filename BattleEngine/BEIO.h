#pragma once

enum class BEIOReportMode
{
    Debug,
    Warning,
    Error
};

#define DEFINE_LOG_OUTPUT(name, mode) \
static void name##Output(const char* BEString) \
{ \
    Output(BEString, #mode); \
}

class BEIO
{
    static void Output(const char* BEString, const char* Mode);
public:
    DEFINE_LOG_OUTPUT(Debug, BEIOReportMode::Debug)
    DEFINE_LOG_OUTPUT(Warn, BEIOReportMode::Warning)
    DEFINE_LOG_OUTPUT(Error, BEIOReportMode::Error)
};
