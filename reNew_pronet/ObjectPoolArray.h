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
		//	�ė��p�\�ȃJ�����g
		uint32_t pointer;
		//	���ۂ̃T�C�Y�A�������ꍇ�͋�ɂȂ莟����
		uint32_t bufsize;
		//	�g�p����Ă���T�C�Y
		uint32_t usedsize;
		//	���σT�C�Y
		uint32_t avgsize;
		uint32_t avgcount;
		//	�A�����ăI�u�W�F�N�g���m�ۂ�����
		uint32_t getcount;
		//	�A�����ăI�u�W�F�N�g�����������
		uint32_t backcount;
		//	�z��̐擪�̃|�C���^
		T* pool_top;

	public:
		//	�R���X�g���N�^
		ObjectPool_Array(size_t size = 16);
		//	�f�X�g���N�^
		~ObjectPool_Array();

		//	�I�u�W�F�N�g���v�[������|�b�v����
		PoolArray<T> get(size_t size);
		//	�I�u�W�F�N�g���v�[���ɕԋp����
		void back(PoolArray<T>* p);
		//	�v�[���̃T�C�Y��ύX����
		void resize(size_t size);
		//	�r�b�g�}�b�v��`�悷��
		void printBitMap() const;

	private:
		inline void compress();
		//	�K�����u���b�N���v�[������T��
		T** search_pool_block(size_t size, size_t* indices);
		//	�r�b�g�}�b�v���g�p���Č�������
		inline uint32_t search_bit_area(uint8_t size);
		//	�|�C���^�i�J�����g�j�����Z�b�g����
		inline void reset_pointer() { pointer = 0; }

		//	�T�C�Y�𕪊����ŃA���C�������g
		inline size_t size_Alingment(size_t size) const;
		//	�T�C�Y�𕪊����Ŋ�������
		inline size_t size_dived_size(size_t size) const;
		//	64�̔{���ŃA���C�������g
		inline size_t paritition_size_Alignment(size_t size) const;
		//	64�Ŋ���������Ԃ�
		inline size_t buf_parirition_size_Alignment(size_t size) const;

		//	�r�b�g�}�b�v��`�悷��
		void printIs_used() const;
		//	�r�b�g��`�悷��
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