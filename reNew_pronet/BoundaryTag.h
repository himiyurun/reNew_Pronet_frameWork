#pragma once
#include <iostream>
#include <cassert>
#include <stdexcept>

namespace pronet {
	
	struct BoundaryTagEnd 
	{
		BoundaryTagEnd(uint32_t size)
			: size(size) {}
		
		uint32_t size;

		~BoundaryTagEnd() {}
	};

	class BoundaryTagBegin
	{
		uint32_t size;
		bool is_used;

		BoundaryTagBegin* next;
		BoundaryTagBegin* prev;

	public:
		BoundaryTagBegin(uint32_t size, bool is_used);

		~BoundaryTagBegin();

		BoundaryTagBegin* split(uint32_t new_size);

		void setSize(uint32_t new_size) { size = new_size; }
		void setUsed(bool now_used) { is_used = now_used; }

		[[nodiscard]] uint32_t bufSize() const { return size; }
		[[nodiscard]] bool used() const { return is_used; }
	};

	//	終わりのタグの作成
	void createEndTag(void* p, uint32_t size) {
		new(p) BoundaryTagEnd(size);
		
		if (!p) { throw std::bad_alloc(); }
	}

	//	初めのタグの作成
	void createBegTag(void* p, uint32_t size, bool used) {
		new(p) BoundaryTagBegin(size, used);

		if (!p) { throw std::bad_alloc(); }
	}

	//	タグの新規作成
	BoundaryTagBegin* createNewTag(void* p, uint32_t size, bool used) {
		createBegTag(p, size, used);

		createEndTag(static_cast<char*>(p) + begSize + size, size);

		return static_cast<BoundaryTagBegin*>(p);
	}

	//	タグの消去
	void deleteTag(void* p) {
		BoundaryTagBegin* begin = reinterpret_cast<BoundaryTagBegin*>(static_cast<char*>(p) - begSize);
		assert(begin && "Memory Error : Delete Pointer is null");

		BoundaryTagEnd* end = reinterpret_cast<BoundaryTagEnd*>(static_cast<char*>(p) + begSize + begin->bufSize());
		assert(end && "Memory Error : Delete Pointer is null");

		begin->~BoundaryTagBegin();
		end->~BoundaryTagEnd();
	}

	//	エンドタグのサイズ
	static const uint8_t endSize = sizeof(BoundaryTagEnd);
	//	ビギンタグのサイズ
	static const uint8_t begSize = sizeof(BoundaryTagBegin);
	//	タグの合計のサイズ
	static const uint8_t tagSize = endSize + begSize;
}