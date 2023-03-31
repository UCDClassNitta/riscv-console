#ifndef MEMORYRANGE_H
#define MEMORYRANGE_H

#include <stdint.h>

class CMemoryRange{
    public:
        uint32_t addr;
	uint32_t range;
	bool operator<(const CMemoryRange & rhs) const;
};

#endif