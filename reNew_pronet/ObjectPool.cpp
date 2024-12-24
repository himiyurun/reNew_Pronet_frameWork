#include "ObjectPool.h"

template<class T>
inline pronet::ObjectPool<T>::ObjectPool(uint32_t size)
	: objpool(0), objlist(0)
	, pointer(0), bufsize(size), usedSize(0)
{
	resize(size);
}

template<class T>
pronet::ObjectPool<T>::~ObjectPool()
{
}

template<class T>
T* pronet::ObjectPool<T>::pop()
{
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

template<class T>
void pronet::ObjectPool<T>::push(T** ptr)
{
	if (!ptr) {
		throw std::runtime_error("return pool obj is null");
	}

	for (T*& a : objlist) {
		if (!a) {
			a = *ptr;
			*ptr = nullptr;
			usedSize--;
		}
	}
}

template<class T>
void pronet::ObjectPool<T>::resize(uint32_t size)
{
	objpool.resize(size);
	objlist.resize(size);

	static uint32_t pointer(0);
	pointer = 0;
	for (T*& list : objlist) {
		objlist[pointer] = &objpool[pointer];
	}
	bufsize = size;
}