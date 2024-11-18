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
	assert(new_size < size - tagSize && "Memory Error : new_size is too big!!");

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

	return rBegin;
}

void pronet::BoundaryTagBegin::marge()
{
	if (next) {
		if (!next->used()) {
			BoundaryTagBegin* rbegin = next;
			BoundaryTagEnd* rend = endTag();

			setNext(next->NextLink());
			size += tagSize + rbegin->bufSize();
			BoundaryTagEnd* end = endTag();
			end->size = size;

			rbegin->~BoundaryTagBegin();
			rend->~BoundaryTagEnd();
		}
	}
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

void pronet::deleteTag(void* p)
{
	BoundaryTagBegin* begin = reinterpret_cast<BoundaryTagBegin*>(static_cast<char*>(p) - begSize);
	assert(begin && "Memory Error : Delete Pointer is null");

	BoundaryTagEnd* end = reinterpret_cast<BoundaryTagEnd*>(static_cast<char*>(p) + begSize + begin->bufSize());
	assert(end && "Memory Error : Delete Pointer is null");

	begin->~BoundaryTagBegin();
	end->~BoundaryTagEnd();
}
