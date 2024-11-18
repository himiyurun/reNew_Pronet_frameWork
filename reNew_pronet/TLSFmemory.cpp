#include "TLSFmemory.h"

using namespace pronet;

TLSFmemory::TLSFmemory(uint32_t n = 7, uint8_t divsize = 4)
	: pool((1 << n) + tagSize + endSize, 0)
	, parititionSLI(n , 0)
	, parititionFLI(0)
	, bufSize(1 << n)
	, divBit(divSize)
	, divSize(1 << divsize)
{
	if (divsize > 4) { 
		divBit = 4;
		divSize = 1 << divBit;
	}

	createBeginTag(&pool[1024 + tagSize], 0, true);
	createNewTag(pool.data(), bufSize, false);
}

pronet::TLSFmemory::~TLSFmemory()
{
}

void pronet::TLSFmemory::calcTlsfIndex(uint8_t* fli, uint8_t* sli, uint32_t size) const
{
	unsigned long index;
	_BitScanReverse(&index, size);
}

void pronet::TLSFmemory::calcSLI(uint8_t* sli, uint8_t fli, uint32_t size) const
{
	static uint8_t shift = 0;
	shift = fli - divBit - 1;
	*sli = (size & ~(1 << fli)) >> (shift);
}
