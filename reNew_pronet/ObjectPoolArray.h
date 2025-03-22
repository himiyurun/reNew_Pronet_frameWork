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
		//	使用されているサイズ
		uint32_t usedsize;
		//	平均サイズ
		uint32_t avgsize;
		uint32_t avgcount;
		//	連続してオブジェクトを確保した回数
		uint32_t getcount;
		//	連続してオブジェクトを解放した回数
		uint32_t backcount;
		//	配列の先頭のポインタ
		T* pool_top;

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
		//	ビットマップを描画する
		void printBitMap() const;

	private:
		inline void compress();
		//	適したブロックをプールから探す
		T** search_pool_block(size_t size, size_t* indices);
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

		[[nodiscard]] bool is_compress() const { 
			if (avgcount) {
				if ((usedsize * 4) < (avgsize / avgcount))
					return true;
			}
			return false;
		}
	};
}

template<typename T>
pronet::ObjectPool_Array<T>::ObjectPool_Array(size_t size)
	: valPool(0)
	, pointer(0), bufsize(size), usedsize(0)
	, avgsize(0), avgcount(0), getcount(0), backcount(0)
	, pool_top(nullptr)
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
	if (backcount != 0) {
		avgsize += usedsize;
		avgcount++;
		backcount = 0;
	}
	PoolArray<T> info;
	info.data = search_pool_block(size_dived_size(size), &info.index);
	info.size = size;
	usedsize++;
	getcount++;
	return info;
}

template<typename T>
void pronet::ObjectPool_Array<T>::back(PoolArray<T>* p)
{
	if (getcount != 0) {
		avgsize += usedsize;
		avgcount++;
		getcount = 0;
		compress();
	}
	size_t bit_size(p->size / 4);
	is_used.write_Bit_0(p->index / 4, bit_size);

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
	usedsize--;
	backcount++;

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
	size_t prevSizeIndex(prevSize / 4);
	size_t nextSize(size_Alingment(size));
	size_t nextSizeIndex(nextSize / 4);
	is_used.write_Bit_0(prevSizeIndex, UNSIGNED_INT_64 - (prevSizeIndex % UNSIGNED_INT_64));
	valPool.resize(nextSize);
	is_used.write_Bit_1(nextSizeIndex, UNSIGNED_INT_64 - (nextSizeIndex % UNSIGNED_INT_64));
	pool_top = valPool.data();
}

template<typename T>
inline void pronet::ObjectPool_Array<T>::printBitMap() const
{
	is_used.printBit();
}

template<typename T>
inline void pronet::ObjectPool_Array<T>::compress()
{
	if (!is_compress())
		return;

	uint32_t cmpsize(usedsize * 2);
	size_t index(0);
	is_used.find_one_from_reverse_l((bufsize / 4) - 1, &index);
	index = (++index) * 4;
	is_used.printBit();
	
	if (index < cmpsize) {
		if (bufsize != index)
			resize(index);
	}
	else {
		if (bufsize != cmpsize)
			resize(cmpsize);
	}
}

template<typename T>
T** pronet::ObjectPool_Array<T>::search_pool_block(size_t size, size_t* indices)
{
	uint32_t index(0);
	index = search_bit_area(size);
	if (index == 0xffffffff) {
		if (size >= bufsize * 2)
			resize(size * 2);
		else
			resize(bufsize * 2);

		index = search_bit_area(size);
		if (index == 0xffffffff) {
			throw std::logic_error("ObjectPool_Array is not work properly : ObjectPool_Array.search_pool_block(size_t, size_t*)");
		}
		std::cerr << "ObjectPool_Array is Full. you called .resize" << std::endl;
	}
	*indices = index * 4;

	return &pool_top;
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
	return (size + POOL_DIVIED_SIZE - 1) & ~(0x03);
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