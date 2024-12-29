#pragma once
#include <iostream>
#include <stdexcept>

#include "bit_tree.h"
#include "pnTlsf.h"

namespace pronet {

	template<class T>
	struct Pool_Object {
		size_t index;
		T* data;

		Pool_Object(size_t index = 0, T* data = nullptr) : index(index), data(data) {}

		template<class T>
		Pool_Object(Pool_Object&& o) noexcept
			: index(o.index), data(o.data) {
			o.index = 0;
			o.data = nullptr;
		}

		T* operator->() {
			return data;
		}

		T* operator()() {
			return data;
		}
	};

	template<class T, std::size_t N>
	class ObjectPool
	{
		std::vector<T, pnTlsfInsertSTLtype<T>> objpool;
		pronet::bit_tree<N> bit_map;

		size_t buf_size;

	public:

		ObjectPool(uint32_t size = 32);

		~ObjectPool();

		pronet::Pool_Object<T> pop();

		void push(Pool_Object<T>* ptr);

		bool get_bit_to_pool_status() const;
		[[nodiscard]] size_t size() const { return buf_size; }

	private:

		void resize(uint32_t size);
	};
}


template<class T, std::size_t N>
inline pronet::ObjectPool<T, N>::ObjectPool(uint32_t size)
	: objpool(0)
	, bit_map(false, 0), buf_size(size)
{
	resize(size);
}

template<class T, std::size_t N>
pronet::ObjectPool<T, N>::~ObjectPool()
{
}

template<class T, std::size_t N>
pronet::Pool_Object<T> pronet::ObjectPool<T, N>::pop()
{
	Pool_Object<T> buf;
	if (bit_map.rigist(&buf.index)) {
		buf.data = &objpool[buf.index];
	}
	else {
		resize(buf_size + (1 << N));
		if (!bit_map.rigist(&buf.index)) {
			throw std::runtime_error("resize process might be failed");
		}
		buf.data = &objpool[buf.index];
	}
	return buf;
}

template<class T, std::size_t N>
void pronet::ObjectPool<T, N>::push(pronet::Pool_Object<T>* ptr)
{
	if (!ptr) {
		throw std::runtime_error("return pool obj is null");
	}

	ptr->data->reset();
	bit_map.unrigist(ptr->index);
	ptr->index = 0;
	ptr->data = 0;
}

template<class T, std::size_t N>
bool pronet::ObjectPool<T, N>::get_bit_to_pool_status() const
{
	std::cout << "bit map" << std::endl;
	bit_map.printAllBit();
	return true;
}

template<class T, std::size_t N>
void pronet::ObjectPool<T, N>::resize(uint32_t size)
{
	objpool.resize(size);
	bit_map.resize(size);
	buf_size = size;
}