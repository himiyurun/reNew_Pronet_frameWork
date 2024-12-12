#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

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

		ObjectPool(uint32_t size = 0)
			: objpool(0), objlist(0)
			, pointer(0), bufsize(size), usedSize(0)
		{
			resize(size);
		}

		~ObjectPool() {
		}

		T* pop() {
			if (usedSize == bufsize) {
				resize(bufsize * 2);
			}

			T* buf = nullptr;
			for (int i = 0; i < bufsize; i++) {
				if (objlist[pointer]) {
					buf = objlist[pointer];
					objlist[pointer] = nullptr;
					usedSize++;
					pointer++;
					break;
				}
				pointer++;
				if (pointer >= objlist.size()) {
					pointer = 0;
				}
			}
			return buf;
		}

		void push(T** ptr) {
			if (!ptr) {
				throw std::runtime_error("return pool obj is null");
			}

			for ( T* &a : objlist ) {
				if (!a) {
					a = *ptr;
					*ptr = nullptr;
					usedSize--;
				}
			}
		}

	private :

		void resize(uint32_t size) {
			objpool.resize(size);
			objlist.resize(size);

			static uint32_t pointer(0);
			pointer = 0;
			for (T*& list : objlist) {
				objlist[pointer] = &objpool[pointer];
			}
			bufsize = size;
		}
	};

	template<typename T>
	class ValPool : public pnTlsf {
		std::vector<T, pnTlsfInsertSTLtype<T>> valPool;
		std::vector<bool, pnTlsfInsertSTLpointer<bool>> valList;

		uint32_t pointer;
		uint32_t bufsize;
		uint32_t usedSize;

	public:

		ValPool(uint32_t size = 0)
			: valPool(0), valList(0)
			, pointer(0), bufsize(size), usedSize(0)
		{
			resize(size);
		}

		~ValPool() {
		}

		T* pop() {
			if (usedSize == bufsize) {
				resize(bufsize * 2);
			}

			T* buf = nullptr;
			for (int i = 0; i < bufsize; i++) {
				if (valList[pointer]) {
					buf = valList[pointer];
					valList[pointer] = nullptr;
					usedSize++;
					pointer++;
					break;
				}
				pointer++;
				if (pointer >= valList.size()) {
					pointer = 0;
				}
			}
			return buf;
		}

		void push(T** ptr) {
			if (!ptr) {
				throw std::runtime_error("return pool obj is null");
			}

			for (T*& a : valList) {
				if (!a) {
					a = *ptr;
					*ptr = nullptr;
					usedSize--;
				}
			}
		}

	private:

		void resize(uint32_t size) {
			valPool.resize(size);
			valList.resize(size);

			static uint32_t pointer(0);
			pointer = 0;
			for (T*& list : valList) {
				valList[pointer] = &valPool[pointer];
			}
			bufsize = size;
		}
	};
}