#pragma once
#include <iostream>
#include <stdexcept>

#include "BitMap64.h"
#include "pnTlsf.h"

namespace pronet {

	template<class T>
	class ObjectPool : public pnTlsf
	{
		std::vector<T, pnTlsfInsertSTLtype<T>> objpool;
		std::vector<T*, pnTlsfInsertSTLpointer<T*>> objlist;

		uint32_t pointer;
		uint32_t bufsize;
		uint32_t usedSize;

	public:

		ObjectPool(uint32_t size = 0);

		~ObjectPool();

		T* pop();

		void push(T** ptr);

	private:

		void resize(uint32_t size);
	};
}