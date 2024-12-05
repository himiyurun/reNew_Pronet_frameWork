#include "Pronet.h"

template<class T>
inline T* new_class(size_t size)
{
	uint8_t* p = static_cast<uint8_t*>(tlsf.allocate(sizeof(T) * size));
	if (!p) {
		throw std::bad_alloc();
	}

	for (int i = 0; i < size; i++) {
		new(p + sizeof(T) * i) T();
	}
	return reinterpret_cast<T*>(p);
}

template<typename T>
inline void delete_class(void* p)
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

	tlsf.deallocate(p);
}

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
{
	object = new_class<Object>(4);
	shader = new_class<Shader>(4);
}

PronetManager::~PronetManager()
{
	delete_class<Object>(object);
	delete_class<Shader>(shader);
}

void PronetManager::process() const
{
	shader[0].use();

	object[0].draw();
}
