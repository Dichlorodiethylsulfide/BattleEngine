#include "BEArray.h"

BETrackedAllocation* allocations = new BETrackedAllocation[ALLOC_COUNT];
size_t totalAllocations = 0;
size_t totalDeallocations = 0;