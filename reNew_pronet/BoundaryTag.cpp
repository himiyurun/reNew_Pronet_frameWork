#include "BoundaryTag.h"

pronet::BoundaryTagBegin::BoundaryTagBegin(uint32_t size = 0, bool is_used = false)
	: size(size), is_used(is_used)
	, next(nullptr), prev(nullptr)
{
}

pronet::BoundaryTagBegin::~BoundaryTagBegin()
{
}

pronet::BoundaryTagBegin* pronet::BoundaryTagBegin::split(uint32_t new_size)
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
	createBegTag(p, rsize, false);

	p += rsize;
	assert(p && "Memory Error : not enough memory!!");
	BoundaryTagEnd* rEnd = reinterpret_cast<BoundaryTagEnd*>(p);

	this->size = new_size;

	rEnd->size = rsize;

	return rBegin;
}
