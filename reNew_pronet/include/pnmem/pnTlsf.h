#ifndef PNTLSF_H_
#define PNTLSF_H_
#include "TLSFmemory.h"

extern pronet::TLSFmemory mainMemory;

namespace pronet {
	extern PNMEM_API void map_memory();

	template<class _Ty>
	extern PNMEM_API inline _Ty* new_type(size_t _size) {
		uint8_t* p = static_cast<uint8_t*>(mainMemory.allocate(sizeof(_Ty) * _size));
		if (!p) {
			throw std::bad_alloc();
		}
		uint8_t* mp = p;
		for (int i = 0; i < _size; i++) {
			new(mp) _Ty();
			mp += sizeof(_Ty);
		}
		return (_Ty*)p;
	}

	template<typename _Ty>
	extern PNMEM_API inline _Ty* new_pointer(size_t _size) {
		uint8_t* p = static_cast<uint8_t*>(mainMemory.allocate(sizeof(_Ty) * _size));
		if (!p) {
			throw std::bad_alloc();
		}
		uint8_t* mp = p;
		for (int i = 0; i < _size; i++) {
			new(mp) _Ty;
			mp += sizeof(_Ty);
		}
		return (_Ty*)p;
	}

	template<typename _Ty>
	extern PNMEM_API inline void delete_type(void* _p) {
		if (!_p)
			throw std::bad_alloc();

		uint8_t* ptr = static_cast<uint8_t*>(_p);
		pronet::BoundaryTagBegin* begin = reinterpret_cast<pronet::BoundaryTagBegin*>(ptr - pronet::begSize);
		if (!begin)
			throw std::bad_alloc();

		_Ty* buf(nullptr);
		for (int i = 0; i < begin->bufSize() / sizeof(_Ty); i++) {
			buf = reinterpret_cast<_Ty*>(ptr + sizeof(_Ty) * i);
			if (!buf)
				throw std::bad_alloc();
			buf->~_Ty();
		}

		mainMemory.deallocate(_p);
	}

	template<typename _Ty>
	extern PNMEM_API void delete_pointer(void* _p) {
		mainMemory.deallocate(_p);
	}

	template<class _Ty = uint8_t>
	class PNMEM_API pnTlsfInsertSTLtype {
	public:
		using value_type = _Ty;
		using value_ptr = _Ty*;
		using size_type = size_t;

		pnTlsfInsertSTLtype() {}

		pnTlsfInsertSTLtype(const pnTlsfInsertSTLtype& _n) {}

		[[nodiscard]] value_ptr allocate(size_type _n) {
			return new_type<value_type>(_n);
		}

		void deallocate(value_ptr _p, size_type _n) {
			delete_type<value_type>(_p);
		}
	};

	template<typename _Ty = uint8_t*>
	class PNMEM_API pnTlsfInsertSTLpointer {
	public:
		using value_type = _Ty;
		using value_ptr = _Ty*;
		using size_type = size_t;

		pnTlsfInsertSTLpointer() {}

		pnTlsfInsertSTLpointer(const pnTlsfInsertSTLpointer&) {}

		[[nodiscard]] value_ptr allocate(size_type _n) {
			return new_pointer<value_type>(_n);
		}

		void deallocate(value_ptr _p, size_type _n) {
			delete_pointer<value_type>(_p);
		}
	};
}
#endif