#ifndef BOUNDARY_TAG_H_
#define BOUNDARY_TAG_H_
#include <iostream>
#include <cassert>
#include <stdexcept>

#define DLL_EXPORTS
#ifdef _WIN32
	#ifdef DLL_EXPORTS
	#define PNMEM_API __declspec(dllexport)
	#else
	#define PNMEM_API __declspec(dllimport)
	#endif
#else
	#define PNMEM_API
#endif

namespace pronet {
	struct PNMEM_API BoundaryTagEnd
	{
		BoundaryTagEnd(size_t size = 0)
			: size(size) {}

		size_t size;

		~BoundaryTagEnd() {}
	};

	class PNMEM_API BoundaryTagBegin
	{
		size_t size;
		bool is_used;

		BoundaryTagBegin* next;
		BoundaryTagBegin* prev;

	public:
		BoundaryTagBegin(size_t size = 0, bool is_used = false);

		~BoundaryTagBegin();

		BoundaryTagBegin* split(size_t new_size);
		void marge(BoundaryTagBegin* next);

		void setSize(size_t new_size) { size = new_size; }
		void setUsed(bool now_used) { is_used = now_used; }

		void setNext(BoundaryTagBegin* new_next) { next = new_next; }
		void setPrev(BoundaryTagBegin* new_prev) { prev = new_prev; }

		//	���W�X�g���Ƀ����N���쐬����
		void attach(BoundaryTagBegin* const _next);
		//	�A�����W�X�g���Ƀ����N����������
		void detach(BoundaryTagBegin*& _tag);

		[[nodiscard]] BoundaryTagEnd* endTag(){
			uint8_t* p = reinterpret_cast<uint8_t*>(this);
			return reinterpret_cast<BoundaryTagEnd*>(p + sizeof(*this) + size);
		}

		[[nodiscard]] BoundaryTagBegin* getNext() const;
		[[nodiscard]] BoundaryTagEnd* getPrev() const;
		[[nodiscard]] BoundaryTagBegin* NextLink() const { return next; }
		[[nodiscard]] BoundaryTagBegin* PrevLink() const { return prev; }
		[[nodiscard]] size_t bufSize() const { return size; }
		[[nodiscard]] bool used() const { return is_used; }
	};

	//	�G���h�^�O�̃T�C�Y
	static const uint8_t endSize = sizeof(BoundaryTagEnd);
	//	�r�M���^�O�̃T�C�Y
	static const uint8_t begSize = sizeof(BoundaryTagBegin);
	//	�^�O�̍��v�̃T�C�Y
	static const uint8_t tagSize = endSize + begSize;

	//	�I���̃^�O�̍쐬
	extern PNMEM_API void createEndTag(void* p, size_t size);

	//	���߂̃^�O�̍쐬
	extern PNMEM_API void createBeginTag(void* p, size_t size, bool used);

	//	�^�O�̐V�K�쐬
	extern PNMEM_API BoundaryTagBegin* createNewTag(void* p, size_t size, bool used);

	//	�^�O�̏���
	extern PNMEM_API void deleteTag(BoundaryTagBegin* begin);

	extern PNMEM_API BoundaryTagBegin* getBegin(BoundaryTagEnd* end);
}
#endif