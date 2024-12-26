#pragma once
#include <iostream>
#include <vector>
#include <cassert>

#include "pnTlsf.h"
#include "BitMap64.h"
#include "ObjectPoolList.h"

#define POOL_DIVIED_SIZE	(4)

namespace pronet {

	template<typename T>
	class ObjectPool_Array {
		std::vector<T, pnTlsfInsertSTLtype<T>> valPool;
		BitMap64 is_used;
		//	再利用可能なカレント
		uint32_t pointer;
		//	実際のサイズ、小さい場合は空になり次第解放
		uint32_t bufsize;

	public:
		//	コンストラクタ
		ObjectPool_Array(size_t size = 16);
		//	デストラクタ
		~ObjectPool_Array();

		//	オブジェクトをプールからポップする
		PoolArray<T> get(size_t size);
		//	オブジェクトをプールに返却する
		void back(PoolArray<T>* p);
		//	プールのサイズを変更する
		void resize(size_t size);

	private:
		//	適したブロックをプールから探す
		T* search_pool_block(size_t size, size_t* indices);
		//	ビットマップを使用して検索する
		inline uint32_t search_bit_area(uint8_t size);
		//	ポインタ（カレント）をリセットする
		inline void reset_pointer() { pointer = 0; }

		//	サイズを分割数でアラインメント
		inline size_t size_Alingment(size_t size) const;
		//	サイズを分割数で割った数
		inline size_t size_dived_size(size_t size) const;
		//	64の倍数でアラインメント
		inline size_t paritition_size_Alignment(size_t size) const;
		//	64で割った数を返す
		inline size_t buf_parirition_size_Alignment(size_t size) const;

		//	ビットマップを描画する
		void printIs_used() const;
		//	ビットを描画する
		void printBit(uint64_t num, uint8_t size) const;
	};
}

template<typename T>
pronet::ObjectPool_Array<T>::ObjectPool_Array(size_t size)
	: valPool(0)
	, is_used(0)
	, pointer(0), bufsize(size)
{
	resize(size);
}

template<typename T>
pronet::ObjectPool_Array<T>::~ObjectPool_Array()
{
}

template<typename T>
pronet::PoolArray<T> pronet::ObjectPool_Array<T>::get(size_t size)
{
	PoolArray<T> info;
	info.data = search_pool_block(size_dived_size(size), &info.index);
	info.size = size;
	return info;
}

template<typename T>
void pronet::ObjectPool_Array<T>::back(PoolArray<T>* p)
{
	size_t bit_size(p->size / 4);
	std::cout << p->index << " " << bit_size << std::endl;
	is_used.write_Bit_0(p->index, bit_size);

	/*
	size_t idx(p->index * 4);
	for (int i = 0; i < p->size; i++) {
		assert(idx < valPool.size() && "Error : ObjectPoolArray : out of range");
		//valPool[idx].reset();
		idx++;
	}
	*/

	p->data = nullptr;
	p->index = 0;
	p->size = 0;

#ifdef _POOL_DEBUG
	is_used.printBit();
#endif
}

template<typename T>
void pronet::ObjectPool_Array<T>::resize(size_t size)
{
	if (size > is_used.size() * POOL_DIVIED_SIZE * 64) {
		is_used.resize(paritition_size_Alignment(size));
	}
	size_t prevSize(valPool.size());
	valPool.resize(size_Alingment(size));
}

template<typename T>
T* pronet::ObjectPool_Array<T>::search_pool_block(size_t size, size_t* indices)
{
	uint32_t index(0);

	index = search_bit_area(size);
	if (index == 0xffffffff) {
		return nullptr;
	}
	*indices = index;

	return &valPool[index * 4];
}

template<typename T>
uint32_t pronet::ObjectPool_Array<T>::search_bit_area(uint8_t size)
{
	bool find_is_block(false);
	size_t top_index(0);
	size_t end_index(0);
	if (!is_used.find_zero_from(0, &top_index)) {
		std::cout << "Log : you should resize pool is full" << std::endl;
		return 0xffffffff;
	}
	if (!is_used.find_one_from(top_index, &end_index)) {
		is_used.write_Bit_1(top_index, size);
		return top_index;
	}
	else {
		if (end_index - top_index >= size) {
			is_used.write_Bit_1(top_index, size);
			return top_index;
		}
	}

	while (!find_is_block) {
		if (!is_used.find_zero_from(top_index + 1, &top_index)) {
			break;
		}
		if (!is_used.find_one_from(top_index, &end_index)) {
			is_used.write_Bit_1(top_index, size);
			return top_index;
		}
		else {
			if (end_index - top_index >= size) {
				is_used.write_Bit_1(top_index, size);
				return top_index;
			}
		}
	}

	std::cout << "Log : you should resize pool, Memory area not found" << std::endl;
	return 0xffffffff;
}

template<typename T>
size_t pronet::ObjectPool_Array<T>::size_Alingment(size_t size) const
{
	if (size % POOL_DIVIED_SIZE == 0) {
		return size;
	}
	else {
		return POOL_DIVIED_SIZE * ((size / POOL_DIVIED_SIZE) + 1);
	}
}

template<typename T>
size_t pronet::ObjectPool_Array<T>::size_dived_size(size_t size) const
{
	if (size % POOL_DIVIED_SIZE == 0) {
		return (size / POOL_DIVIED_SIZE);
	}
	else {
		return ((size / POOL_DIVIED_SIZE) + 1);
	}
}

template<typename T>
size_t pronet::ObjectPool_Array<T>::paritition_size_Alignment(size_t size) const
{
	if (size % 64 == 0) {
		return size / 64;
	}
	else {
		return ((size / 64) + 1);
	}
}

template<typename T>
size_t pronet::ObjectPool_Array<T>::buf_parirition_size_Alignment(size_t size) const
{
	if (size % 64 == 0) {
		return (size / 64);
	}
	else {
		return ((size / 64) + 1);
	}
}

template<typename T>
void pronet::ObjectPool_Array<T>::printIs_used() const
{
	for (uint64_t a : is_used) {
		printBit(a, 64);
	}
}

template<typename T>
void pronet::ObjectPool_Array<T>::printBit(uint64_t num, uint8_t size) const
{
	for (int i = 0; i < size; i++) {
		if (num & (static_cast<uint64_t>(1) << i))
			std::cout << "1";
		else
			std::cout << "0";
	}
	std::cout << std::endl;
}