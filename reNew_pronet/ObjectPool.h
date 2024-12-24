#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <cassert>

#include "pnTlsf.h"

#define POOL_DIVIED_SIZE	(4)

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
	struct divList {
		T* ptr;			//	プールのポインタ

		divList(T* data = nullptr) : ptr(data) {}
	};

	template<typename T>
	struct PoolArray {
		T* data;		//	プールのポインタ
		size_t size;	//	占有しているリストのサイズ

		PoolArray(T* data = nullptr, size_t size = 0) : data(data), size(size) {}
	};

	template<typename T>
	class ValPool : public pnTlsf {
		std::vector<T, pnTlsfInsertSTLtype<T>> valPool;
		std::vector<divList<T>, pnTlsfInsertSTLpointer<divList<T>>> poolList;
		std::vector<uint64_t> is_used;

		uint32_t pointer;
		uint32_t bufsize;

	public:
		//	コンストラクタ
		ValPool(size_t size = 16)
			: valPool(0)
			, poolList(0)
			, is_used(0)
			, pointer(0), bufsize(size)
		{
			resize(size);
		}
		
		~ValPool() {
		}

		//	オブジェクトをプールからポップする
		PoolArray<T> pop(size_t size) {
			PoolArray<T> info;
			info.data = search_pool_block(size_dived_size(size));
			info.size = size_dived_size(size);
			return info;
		}

		//	プールのサイズを変更する
		void resize(size_t size) {
			if (size > is_used.size() * POOL_DIVIED_SIZE * 64) {
				is_used.resize(paritition_size_Alignment(size), 0);
			}
			size_t prevSize(valPool.size());
			valPool.resize(size_Alingment(size));
			poolList.resize(size_Alingment(size) / POOL_DIVIED_SIZE);
			resize_rigist_list(prevSize, valPool.size());
		}

	private:

		T* search_pool_block(size_t size) {
			T* buf(nullptr);

			uint32_t index(serch_block_Bit(size));
			std::cout << "size : " << index << std::endl;

			return &valPool[index];
		}

		inline uint32_t serch_block_Bit(uint8_t size) {
			uint8_t array_size(buf_parirition_size_Alignment(size));
			assert(array_size > 0 && "Error : Array_Size is 0");
			uint8_t bit_size(size);
			std::unique_ptr<uint64_t[]> bitArray(new uint64_t[array_size]);

			for (int i = 0; i < array_size; i++) {
				bitArray[i] = 0;
			}
			
			pointer = 0;
			for (int i = 0; i < bit_size; i++) {
				if (i % 64 == 0) {
					pointer++;
				}
				write_bit(&bitArray[pointer - 1], i - (16 * (pointer - 1)), true);
			}
			reset_pointer();

			for (int i = 0; i < array_size; i++) {
				printBit(bitArray[i], 64);
			}

			pointer = 0;
			bool block_is_found(false);
			uint32_t point, index(0);
			uint32_t bufIndex(0);
			printIs_used();
			while (!block_is_found) {
				std::cout << "pointer : " << pointer << std::endl;
				std::cout << "is_used buf size : " << is_used.size() << std::endl;
				if (pointer >= is_used.size()) {
					throw std::out_of_range("Object_Pool is full");
				}
				block_is_found = true;
				point = pointer;
				for (int i = 0; i < array_size; i++) {
					if (is_used[pointer] == 0xffffffffffffffff) {
						pointer++;
						block_is_found = false;
						break;
					}
					if (!serch_bit_Area(is_used[pointer], bit_size - 64 * ((array_size - 1) - i), &bufIndex)) {
						block_is_found = false;
						pointer++;
#ifdef _POOL_DEBUG
						std::cout << "ObjectPool >> not found free block" << std::endl;
#endif
						break;
					}
#ifdef _POOL_DEBUG
					else {
						std::cout << "objectPool >> found free block" << std::endl;
					}
#endif
					pointer++;
					if (i == 0) index = bufIndex;
				}
			}
			reset_pointer();

			pointer = point;
			for (int i = 0; i < bit_size; i++) {
				for (int j = 0; j < 4; j++) {
					write_bit(&is_used[pointer], (index + i) * 4 + j - 64 * (pointer - point), true);
				}
				if (index + i >= 64) {
					pointer++;
				}
			}
			return 64 * point + index;
		}

		//	ビットの領域を検索
		bool serch_bit_Area(uint64_t n, uint8_t bitSize, uint32_t *index) const {
			uint64_t serch_mask(0);
			if (bitSize == 63) {
				serch_mask = 0xffffffffffffffff;
			}
			else {
				serch_mask |= ~(1 << (bitSize + 1));
			}

			for (int i = 0; i <= 64 - bitSize; i++) {
				if ((n & (serch_mask << i)) >> i == 0) {
					*index = i;
					return true;
				}
			}
			return false;
		}

		//	ビットにデータを書き込み
		inline void write_bit(uint64_t *n, size_t point, bool param) const {
			uint64_t i(1);
			assert(point < 64 && "Error : write_bit point is out of lange");
			if (param == true) {
				*n |= i << point;
			}
			else {
				*n &= ~(i << (point + 1));
			}
		}

		//	リストにプールのデータを同期する
		//	offset : データの同期を始める位置
		//	size : プールの変更したサイズ
		inline void resize_rigist_list(size_t offset, size_t size) {
			pointer = offset / POOL_DIVIED_SIZE;
			for (size_t i = offset; i < size; i++) {
				if (i % POOL_DIVIED_SIZE == 0) {
					poolList[pointer].ptr = &valPool[i];
					pointer++;
				}
			}
			reset_pointer();
		}

		inline void reset_pointer() {
			pointer = 0;
		}

		//	サイズを分割数でアラインメント
		inline size_t size_Alingment(size_t size) const {
			if (size % POOL_DIVIED_SIZE == 0) {
				return size;
			}
			else {
				return POOL_DIVIED_SIZE * ((size / POOL_DIVIED_SIZE) + 1);
			}
		}

		inline size_t size_dived_size(size_t size) {
			if (size % POOL_DIVIED_SIZE == 0) {
				return (size / POOL_DIVIED_SIZE);
			}
			else {
				return ((size / POOL_DIVIED_SIZE) + 1);
			}
		}

		//	64の倍数でアラインメント
		inline size_t paritition_size_Alignment(size_t size) const {
			if (size % 64 == 0) {
				return size / 64;
			}
			else {
				return ((size / 64) + 1);
			}
		}

		//	64で割った数を返す
		inline size_t buf_parirition_size_Alignment(size_t size) const {
			if (size % 64 == 0) {
				return (size / 64);
			}
			else {
				return ((size / 64) + 1);
			}
		}

		void printIs_used() {
			for (uint64_t a : is_used) {
				printBit(a, 64);
			}
		}

		void printBit(uint64_t num, uint8_t size) const
		{
			for (int i = 0; i < size; i++) {
				if (num & (static_cast<uint64_t>(1) << i))
					std::cout << "1";
				else
					std::cout << "0";
			}
			std::cout << std::endl;
		}
	};
}