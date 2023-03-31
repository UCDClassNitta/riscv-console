#include "MemoryRange.h"

bool CMemoryRange::operator<(const CMemoryRange & rhs) const {
    return (addr + range - 1) < rhs.addr;
}
