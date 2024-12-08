#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

#include "pnTlsf.h"

namespace pronet {

	template<class T>
	class ObjectPool : public pnTlsf
	{
		T* objpool;
		T** objlist;

		uint32_t pointer;
		uint32_t bufsize;
		uint32_t usedSize;

	public:

		ObjectPool(uint32_t size)
			: pointer(0), bufsize(size), usedSize(0)
		{
			objpool = new_class<T>(size);
			objlist = new_class<T*>(size);
			for (int i = 0; i < bufsize; i++) {
				*(objlist + i) = objpool + i;
			}
		}

		~ObjectPool() {
			if(objpool)
				delete_class<T>(objpool);
			if (objlist)
				delete_class<T>(objlist);
		}

		T* pop() {
			if (usedSize == bufsize)
				return nullptr;

			while (*(objlist + pointer) == nullptr) {
				pointer++;
				if (pointer >= bufsize) {
					pointer = 0;
				}
			}
			usedSize++;
			return std::move(*(objlist + pointer));
		}

		void push(T** ptr) {
			if (!ptr) {
				throw std::runtime_error("return pool obj is null");
			}

			for (int i = 0; i < bufsize; i++) {
				if (*(objlist + i) != nullptr) {
					*(objlist + i) = std::move(*ptr);
					break;
				}
			}
		}

	private :

		void resize(uint32_t size) {
			T* buf = new_class<T>(size);
			T** listbuf = new_class<T*>(size);

			if (buf) {
				if (bufsize < size)
					bufsize = size;

				for (int i = 0; i < bufsize; i++) {
					*(buf + i) = *(objpool + i);
					*(listbuf + i) = buf + i;
				}
				delete_class<T>(objpool);
				delete_class<T>(objlist);
				bufsize = size;
				objpool = buf;
				objlist = listbuf;
			}
			else {
				std::cerr << "memory Pool is full" << std::endl;
			}
		}
	};
}

