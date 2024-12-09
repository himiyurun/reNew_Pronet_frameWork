#pragma once
#include <new>

#include "TLSFmemory.h"

#define POOL_SIZE_BIT	(11)
#define POOL_INDEX_BIT	(4)

static pronet::TLSFmemory mainMemory(POOL_SIZE_BIT, POOL_INDEX_BIT);

class pnTlsf {

public:

	void* operator new(size_t size) {
		void* ptr = mainMemory.allocate(size);
		std::cout << "operator new" << std::endl;
		if (!ptr)
			throw std::bad_alloc();
		return ptr;
	}

	void* operator new[](size_t size) {
		void* ptr = mainMemory.allocate(size);
		std::cout << "operator new[]" << std::endl;
		if (!ptr)
			throw std::bad_alloc();
		return ptr;
	}

	void operator delete(void* p) {
		std::cout << "operator delete" << std::endl;
		mainMemory.deallocate(p);
	}

	void operator delete[](void* p) {
		std::cout << "operator delete[]" << std::endl;
		mainMemory.deallocate(p);
	}

	template<class T>
	inline T* new_type(size_t size)
	{
		uint8_t* p = static_cast<uint8_t*>(mainMemory.allocate(sizeof(T) * size));
		if (!p) {
			throw std::bad_alloc();
		}
		uint8_t* mp = p;
		for (int i = 0; i < size; i++) {
			new(mp) T();
			mp += sizeof(T);
		}
		return reinterpret_cast<T*>(p);
	}

	template<typename T>
	inline T* new_pointer(size_t size) {
		uint8_t* p = static_cast<uint8_t*>(mainMemory.allocate(sizeof(T) * size));
		if (!p) {
			throw std::bad_alloc();
		}
		uint8_t* mp = p;
		for (int i = 0; i < size; i++) {
			new(mp) T;
			mp += sizeof(T);
		}
		return reinterpret_cast<T*>(p);
	}

	template<typename T>
	inline void delete_type(void* p)
	{
		if (!p)
			throw std::bad_alloc();

		uint8_t* ptr = static_cast<uint8_t*>(p);
		pronet::BoundaryTagBegin* begin = reinterpret_cast<pronet::BoundaryTagBegin*>(ptr - pronet::begSize);
		if (!begin)
			throw std::bad_alloc();

		T* buf(nullptr);
		for (int i = 0; i < begin->bufSize() / sizeof(T); i++) {
			buf = reinterpret_cast<T*>(ptr + sizeof(T) * i);
			if (!buf)
				throw std::bad_alloc();
			buf->~T();
		}

		mainMemory.deallocate(p);
	}

	template<typename T>
	void delete_pointer(void* p) {
		mainMemory.deallocate(p);
	}
}; 

template<class T>
class pnTlsfInsertSTLtype : public pnTlsf {
public : 

	using value_type = T;
	using value_ptr = T*;
	using size_type = size_t;

	pnTlsfInsertSTLtype() = default;

	template <typename U>
	pnTlsfInsertSTLtype(const pnTlsfInsertSTLtype<U>&) {}

	[[nodiscard]] value_ptr allocate(size_type n) {
		std::cout << "Class allocate : " << n << std::endl;
		return new_type<value_type>(n);
	}

	void deallocate(value_ptr p, size_type n) {
		std::cout << "Class deallocate" << std::endl;
		delete_type<value_type>(p);
	}
};

template<typename T>
class pnTlsfInsertSTLpointer : public pnTlsf {
public:

	using value_type = T;
	using value_ptr = T*;
	using size_type = size_t;

	pnTlsfInsertSTLpointer() = default;

	template <typename U>
	pnTlsfInsertSTLpointer(const pnTlsfInsertSTLpointer<U>&) {}

	[[nodiscard]] value_ptr allocate(size_type n) {
		std::cout << "Class allocate : " << n << std::endl;
		return new_pointer<value_type>(n);
	}

	void deallocate(value_ptr p, size_type n) {
		std::cout << "Class deallocate" << std::endl;
		delete_pointer<value_type>(p);
	}
};