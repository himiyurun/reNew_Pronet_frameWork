#include "BoundaryTag.h"

using namespace pronet;

BoundaryTagBegin::BoundaryTagBegin(uint32_t size = 0, bool is_used = false)
	: size(size), is_used(is_used)
	, next(nullptr), prev(nullptr)
{
}

BoundaryTagBegin::~BoundaryTagBegin()
{
}

BoundaryTagBegin* BoundaryTagBegin::split(uint32_t new_size)
{
	static uint8_t* p = nullptr;

	if (new_size <= 0) { return nullptr; }
	if (new_size >= size - tagSize) { return nullptr; }

	uint8_t rsize(size - new_size - tagSize);

	p = reinterpret_cast<uint8_t*>(this);
	p += begSize + new_size;
	createEndTag(p, new_size);

	p += endSize;
	assert(p && "Memory Error : not enough memory!!");
	BoundaryTagBegin* rBegin = reinterpret_cast<BoundaryTagBegin*>(p);
	createBeginTag(p, rsize, false);

	p += rsize;
	assert(p && "Memory Error : not enough memory!!");
	BoundaryTagEnd* rEnd = reinterpret_cast<BoundaryTagEnd*>(p);

	this->size = new_size;
	rEnd->size = rsize;

	
	rBegin->setNext(next);
	this->setNext(rBegin);
	rBegin->setPrev(this);
	
	/*
	next->setPrev(prev);
	prev->setNext(next);
	next = nullptr;
	prev = nullptr;
	*/

	return rBegin;
}

void pronet::BoundaryTagBegin::marge(BoundaryTagBegin* next)
{
	if (next) {
		if (!next->used()) {
			BoundaryTagBegin* rbegin = next;
			BoundaryTagEnd* rend = next->endTag();

			setNext(next->NextLink());
			size += tagSize + rbegin->bufSize();
			BoundaryTagEnd* end = endTag();
			rend->size = size;

			rbegin->~BoundaryTagBegin();
			end->~BoundaryTagEnd();
		}
	}
	/*
	if (prev) {
		if (!prev->used()) {
			BoundaryTagBegin* lbegin = prev;
			BoundaryTagEnd* lend = endTag();

			setPrev(prev->PrevLink());
			size += tagSize + lbegin->bufSize();
			BoundaryTagEnd* end = endTag();
			end->size = size;

			lbegin->~BoundaryTagBegin();
			lend->~BoundaryTagEnd();
		}
	}
	*/
}

BoundaryTagBegin* pronet::BoundaryTagBegin::getNext() const
{
	uint8_t* ptr = reinterpret_cast<uint8_t*>(const_cast<BoundaryTagBegin*>(this));
	//	äÎÇ»Ç¢ÅIÅI
	ptr += size + tagSize;
	return reinterpret_cast<BoundaryTagBegin*>(ptr);
}

BoundaryTagEnd* pronet::BoundaryTagBegin::getPrev() const
{
	uint8_t* ptr = reinterpret_cast<uint8_t*>(const_cast<BoundaryTagBegin*>(this));
	ptr -= endSize;
	BoundaryTagEnd* prevEnd = reinterpret_cast<BoundaryTagEnd*>(ptr);
	return reinterpret_cast<BoundaryTagEnd*>(ptr);
}

void pronet::createEndTag(void* p, uint32_t size)
{
	new(p) BoundaryTagEnd(size);

	if (!p) { throw std::bad_alloc(); }
}

void pronet::createBeginTag(void* p, uint32_t size, bool used)
{
	new(p) BoundaryTagBegin(size, used);

	if (!p) { throw std::bad_alloc(); }
}

BoundaryTagBegin* pronet::createNewTag(void* p, uint32_t size, bool used)
{
	createBeginTag(p, size, used);
	createEndTag(static_cast<char*>(p) + begSize + size, size);

	return static_cast<BoundaryTagBegin*>(p);
}

void pronet::deleteTag(BoundaryTagBegin* begin)
{
	assert(begin && "Memory Error : Delete Pointer is null");

	BoundaryTagEnd* end = begin->endTag();
	assert(end && "Memory Error : Delete Pointer is null");

	begin->~BoundaryTagBegin();
	end->~BoundaryTagEnd();
}

BoundaryTagBegin* pronet::getBegin(BoundaryTagEnd* end)
{
	uint8_t* ptr = reinterpret_cast<uint8_t*>(end);
	return reinterpret_cast<BoundaryTagBegin*>(ptr - end->size - begSize);
}
