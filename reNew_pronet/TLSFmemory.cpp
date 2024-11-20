#include "TLSFmemory.h"

using namespace pronet;

TLSFmemory::TLSFmemory(uint32_t n = 7, uint8_t divsize = 4)
	: pool((1 << n) + tagSize + endSize, 0)
	, parititionSLI(n + 1, 0)
	, parititionFLI(0)
	, bufSize(1 << n)
	, divBit(divSize)
	, divSize(1 << divsize)
{
	if (divsize > 4) { 
		divBit = 4;
		divSize = 1 << divBit;
	}

	createBeginTag(pool.data() + tagSize + bufSize, 0, true);
	createNewTag(pool.data(), bufSize, false);

	writeParititionBit(bufSize);
	std::cout << "FLI : ";
	printBit(parititionFLI, 64);
	for (uint16_t a : parititionSLI) {
		std::cout << "SLI : ";
		printBit(a, 16);
	}

	clearParititionBit(bufSize);
	std::cout << "FLI : ";
	printBit(parititionFLI, 64);
	for (uint16_t a : parititionSLI) {
		std::cout << "SLI : ";
		printBit(a, 16);
	}
}

pronet::TLSFmemory::~TLSFmemory()
{
	BoundaryTagBegin* dummy = reinterpret_cast<BoundaryTagBegin*>(pool.data() + tagSize + pool.size());
	dummy->~BoundaryTagBegin();

}

void pronet::TLSFmemory::writeParititionBit(uint32_t size)
{
	uint8_t fli = 0;
	uint8_t sli = 0;
	calcTlsfIndex(&fli, &sli, size);

	writeBit(fli, sli);
}

void pronet::TLSFmemory::clearParititionBit(uint32_t size)
{
	uint8_t fli = 0;
	uint8_t sli = 0;
	calcTlsfIndex(&fli, &sli, size);
	
	clearBit(fli, sli);
}

void pronet::TLSFmemory::writeBit(uint8_t fli, uint8_t sli)
{
	if (!fli_used(fli))
		parititionFLI |= static_cast<uint64_t>(1) << fli;

	if (!sli_used(fli, sli))
		parititionSLI[fli] |= (1 << sli);
}

void pronet::TLSFmemory::clearBit(uint8_t fli, uint8_t sli)
{
	if (sli_used(fli, sli))
		parititionSLI[fli] &= ~(static_cast<uint16_t>(1) << sli);

	if (!parititionSLI[fli]) {}
		parititionFLI &= ~(static_cast<uint64_t>(1) << fli);
}

void pronet::TLSFmemory::calcTlsfIndex(uint8_t* fli, uint8_t* sli, uint32_t size) const
{
	unsigned long index;
	_BitScanReverse(&index, size);
	*fli = static_cast<uint8_t>(index);
	calcSLI(sli, *fli, size);
}

void pronet::TLSFmemory::calcSLI(uint8_t* sli, uint8_t fli, uint32_t size) const
{
	static uint8_t shift = 0;
	shift = fli - divBit;
	*sli = (size & ~(1 << fli)) >> (shift);
}

void pronet::TLSFmemory::printBit(uint64_t num, uint8_t size) const
{
	for (int i = 0; i < size; i++) {
		if (num & (static_cast<uint64_t>(1) << i))
			std::cout << "1";
		else
			std::cout << "0";
	}
	std::cout << std::endl;
}

bool pronet::TLSFmemory::fli_used(uint8_t fli) const
{
	if (parititionFLI & static_cast<uint64_t>(1) << fli)
		return true;
	return false;
}

bool pronet::TLSFmemory::sli_used(uint8_t fli, uint8_t sli) const
{
	if (parititionSLI[fli] & (1 << sli))
		return true;
	return false;
}
