#include "TLSFmemory.h"

using namespace pronet;

TLSFmemory::TLSFmemory(uint32_t n = 7, uint8_t divsize = 4)
	: pool((1 << n) + (tagSize * 2), 0)
	, freelist((1 << divsize) * n, nullptr)
	, parititionSLI(n + 1, 0)
	, parititionFLI(0)
	, bufSize(1 << n)
	, divBit(divsize)
	, divSize(1 << divsize)
{
	if (divsize > 4) { 
		divBit = 4;
		divSize = 1 << divBit;
	}
	std::cout << "pool size : " << bufSize << std::endl;

	createEndTag(pool.data(), 0xffffffff);
	createBeginTag(pool.data() + tagSize + bufSize, 0, true);
	BoundaryTagBegin* begin = createNewTag(pool.data() + endSize, bufSize, false);

	rigist(begin, bufSize + tagSize);
	std::cout << "FLI : ";
	printBit(parititionFLI, 64);
	for (uint16_t a : parititionSLI) {
		std::cout << "SLI : ";
		printBit(a, 16);
	}
}

pronet::TLSFmemory::~TLSFmemory()
{
	for (BoundaryTagBegin*& a : freelist) {
		if(a)
			deleteTag(a);
	}

	BoundaryTagBegin* dummy = reinterpret_cast<BoundaryTagBegin*>(pool.data() + tagSize + bufSize);
	dummy->~BoundaryTagBegin();
}

void* pronet::TLSFmemory::allocate(uint32_t size)
{
	uint32_t bufsize(sizeAlignment(size));
	//std::cout << "size : "<<size << std::endl;
	if (bufsize < minSize) { bufsize = minSize; }
	bufsize += tagSize;
	//std::cout << "bufsize : " << bufsize << std::endl;

	uint8_t fli, sli;
	calcTlsfIndex(&fli, &sli, bufsize);
	//std::cout << "fli : " << static_cast<unsigned>(fli) << ", sli : " << static_cast<unsigned>(sli) << std::endl;

	BoundaryTagBegin* begin = searchFreeBlock(fli, sli);
	if (!begin) { throw std::runtime_error("tag is null"); }

	if (!begin) { throw std::runtime_error("tag is null"); }
	unrigist(begin, begin->bufSize() + tagSize);
	
	BoundaryTagBegin* lbegin = begin->split(sizeAlignment(size));
	if (lbegin) {
		std::cout << lbegin->bufSize() << std::endl;
		rigist(lbegin, lbegin->bufSize());
	}

	std::cout << "FLI : ";
	printBit(parititionFLI, 64);
	for (uint16_t a : parititionSLI) {
		std::cout << "SLI : ";
		printBit(a, 16);
	}
	
	begin->setUsed(true);

	return reinterpret_cast<uint8_t*>(begin) + begSize;
}

BoundaryTagBegin* pronet::TLSFmemory::searchFreeBlock(uint8_t fli, uint8_t sli) const
{
	uint8_t f = fli, s = sli;
	if (!getLSB(parititionSLI[fli] & ~((1 << sli) - 1), &s)) {
		if (!getLSB(parititionFLI & ~((1 << fli) - 1), &f)) {
			throw std::runtime_error("Memory Pool is Full");
			return nullptr;
		}
		if (!getLSB(parititionSLI[f], &s)) {
			throw std::runtime_error("Not work paritition Bit SLI or FLI");
			return nullptr;
		}
	}
	return freelist[calcIndex(f, s)];
}

void pronet::TLSFmemory::deallocate(void* p)
{
	uint8_t* ptr = static_cast<uint8_t*>(p);
	BoundaryTagBegin* begin = reinterpret_cast<BoundaryTagBegin*>(ptr - begSize);
	begin->setUsed(false);

	if (!margeNextfreeBlock(begin, begin->NextLink()) && !margePrevfreeBlock(begin->PrevLink(), begin)) {
		rigist(begin, begin->bufSize());
	}

	std::cout << "FLI : ";
	printBit(parititionFLI, 64);
	for (uint16_t a : parititionSLI) {
		std::cout << "SLI : ";
		printBit(a, 16);
	}
}

bool pronet::TLSFmemory::margeNextfreeBlock(BoundaryTagBegin* begin, BoundaryTagBegin* next)
{
	if (begin->getNext()) {
		if (!next->used()) {
			unrigist(next, next->bufSize());
			begin->marge(next);
			rigist(begin, begin->bufSize());
			return true;
		}
	}
	return false;
}

bool pronet::TLSFmemory::margePrevfreeBlock(BoundaryTagBegin* prev, BoundaryTagBegin* begin)
{
	if (prev) {
		if (!prev->used()) {
			unrigist(begin, begin->bufSize());
			prev->marge(begin);
			rigist(prev, prev->bufSize());
			return true;
		}
	}
	return false;
}

void pronet::TLSFmemory::rigist(BoundaryTagBegin* begin, uint32_t size)
{
	uint8_t fli, sli;
	calcTlsfIndex(&fli, &sli, size);
	std::cout << "fli : " << static_cast<unsigned>(fli) << ", sli : " << static_cast<unsigned>(sli) << std::endl;
	std::cout << "calc : " << calcIndex(fli, sli) << ", " << divSize * fli + sli << std::endl;
	freelist[calcIndex(fli, sli)] = begin;
	writeBit(fli, sli);
}

void pronet::TLSFmemory::unrigist(BoundaryTagBegin* begin, uint32_t size)
{
	uint8_t fli, sli;
	calcTlsfIndex(&fli, &sli, size);

	freelist[calcIndex(fli, sli)] = nullptr;
	clearBit(fli, sli);
}

void pronet::TLSFmemory::writeParititionBit(uint8_t fli, uint8_t sli)
{
	writeBit(fli, sli);
}

void pronet::TLSFmemory::clearParititionBit(uint8_t fli, uint8_t sli)
{	
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

bool pronet::TLSFmemory::getLSB(uint64_t num, uint8_t *lsb) const
{
	if (num == 0) return false;
	
	unsigned long l = 0;
	_BitScanForward64(&l, num);
	*lsb = static_cast<uint8_t>(l);
	return true;
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
