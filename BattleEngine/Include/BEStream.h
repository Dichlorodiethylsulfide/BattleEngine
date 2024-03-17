#pragma once

#include "BETypeTraits.h"

#if BE_USE_STD_STREAM
#include <iostream>
#if UNICODE
using SOStream = std::wostream;
inline SOStream& COut = std::wcout;
#else
using SOStream = std::ostream;
inline SOStream& COut = std::cout;
#endif
#else
#endif
