#pragma once
#include <iostream>
#include <stdexcept>

#include "bit_tree.h"
#include "pnTlsf.h"

#define COMPRESSABLE_OFFSET	(4)

namespace pronet {

	template<class T>
	struct Pool_Object {
		size_t index;
		T** data;

		Pool_Object(size_t index = 0, T** data = nullptr) : index(index), data(data) {
		}

		template<class T>
		Pool_Object(Pool_Object&& o) noexcept
			: index(o.index), data(o.data) {
			o.index = 0;
			o.data = nullptr;
		}

		T* operator->() {
			return &(*data)[index];
		}

		T* operator()() {
			return &(*data)[index];
		}
	};

	template<class T, std::size_t N>
	class ObjectPool
	{
		std::vector<T, pnTlsfInsertSTLtype<T>> objpool;
		T* pool_top;

		pronet::bit_tree<N> bit_map;
		size_t buf_size;
		size_t used_size;

	public:

		ObjectPool(uint32_t size = 32);

		~ObjectPool();
		//	オブジェクトを検索し受け取る
		pronet::Pool_Object<T> pop();
		//	オブジェクトを返却する
		void push(Pool_Object<T>* ptr);
		//	オブジェクト検索用のビットマップを描画する
		bool get_bit_to_pool_status() const;
		[[nodiscard]] size_t size() const { return buf_size; }
		//	サイズを変更する
		void resize(size_t size);
	private:

		//	末尾にあるオブジェクトの位置を検索し、適切な大きさになるようにサイズを小さくする
		void compre();
		[[nodiscard]] bool is_compred() const;
	};
}


template<class T, std::size_t N>
inline pronet::ObjectPool<T, N>::ObjectPool(uint32_t size)
	: objpool(0), pool_top(nullptr)
	, bit_map(false, 0), buf_size(size)
	, used_size(0)
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
		buf.data = &pool_top;
	}
	else {
		resize(buf_size + (1 << N));
		if (!bit_map.rigist(&buf.index)) {
			throw std::runtime_error("resize process must be failed");
		}
		buf.data = &pool_top;
	}
	used_size++;
	return buf;
}

template<class T, std::size_t N>
void pronet::ObjectPool<T, N>::push(pronet::Pool_Object<T>* ptr)
{
	if (!ptr) {
		throw std::runtime_error("return pool obj is null");
	}

	ptr->operator->()->reset();
	bit_map.unrigist(ptr->index);
	ptr->index = 0;
	ptr->data = nullptr;
	used_size--;
	if (used_size)
		compre();
	else
		resize(buf_size / 2);
}

template<class T, std::size_t N>
bool pronet::ObjectPool<T, N>::get_bit_to_pool_status() const
{
	std::cout << "bit map" << std::endl;
	bit_map.printAllBit();
	return true;
}

template<class T, std::size_t N>
void pronet::ObjectPool<T, N>::resize(size_t size)
{
	std::cout << "resize : " << size << ", prev size : " << buf_size << std::endl;
	objpool.resize(size);
	bit_map.resize(size);
	buf_size = size;
	pool_top = objpool.data();
}

template<class T, std::size_t N>
inline void pronet::ObjectPool<T, N>::compre()
{
	if (!is_compred())
		return;
	std::cout << "compare" << std::endl;
	size_t index(0);
	size_t cmp_size(buf_size / 2);
	assert(cmp_size && "Error : diff size is 0 : ObjectPool.compare()");
	//	圧縮できる最大サイズを求める
	std::cout << "wish size : " << cmp_size << std::endl;
	bit_map.compress(&index, cmp_size);
	std::cout << "compress_size : " << index << std::endl;
	resize(index);
}

template<class T, std::size_t N>
inline bool pronet::ObjectPool<T, N>::is_compred() const
{
	if ((used_size * COMPRESSABLE_OFFSET) < buf_size) {
		return true;
	}
	return false;
}
