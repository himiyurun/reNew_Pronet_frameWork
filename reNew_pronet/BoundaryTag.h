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
		void marge(BoundaryTagBegin* next);

		void setSize(uint32_t new_size) { size = new_size; }
		void setUsed(bool now_used) { is_used = now_used; }

		void setNext(BoundaryTagBegin* new_next) { next = new_next; }
		void setPrev(BoundaryTagBegin* new_prev) { prev = new_prev; }

		[[nodiscard]] BoundaryTagEnd* endTag(){
			uint8_t* p = reinterpret_cast<uint8_t*>(this);
			return reinterpret_cast<BoundaryTagEnd*>(p + sizeof(*this) + size);
		}

		[[nodiscard]] BoundaryTagBegin* getNext() const;
		[[nodiscard]] BoundaryTagEnd* getPrev() const;
		[[nodiscard]] BoundaryTagBegin* NextLink() const { return next; }
		[[nodiscard]] BoundaryTagBegin* PrevLink() const { return prev; }
		[[nodiscard]] uint32_t bufSize() const { return size; }
		[[nodiscard]] bool used() const { return is_used; }
	};

	//	エンドタグのサイズ
	static const uint8_t endSize = sizeof(BoundaryTagEnd);
	//	ビギンタグのサイズ
	static const uint8_t begSize = sizeof(BoundaryTagBegin);
	//	タグの合計のサイズ
	static const uint8_t tagSize = endSize + begSize;

	//	終わりのタグの作成
	void createEndTag(void* p, uint32_t size);

	//	初めのタグの作成
	void createBeginTag(void* p, uint32_t size, bool used);

	//	タグの新規作成
	BoundaryTagBegin* createNewTag(void* p, uint32_t size, bool used);

	//	タグの消去
	void deleteTag(BoundaryTagBegin* begin);

	BoundaryTagBegin* getBegin(BoundaryTagEnd* end);
}