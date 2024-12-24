#pragma once
#include <iostream>
#include <vector>
#include <cassert>

#include "pnTlsf.h"
#include "BitMap64.h"

namespace pronet {

#define POOL_DIVIED_SIZE	(4)

	template<typename T>
	struct PoolArray {
		T* data;		//	�v�[���̃|�C���^
		size_t index;	//	�r�b�g�}�b�v�̃C���f�b�N�X
		size_t size;	//	���ۂ̃T�C�Y

		PoolArray(T* data = nullptr, size_t size = 0);

		T& operator[](size_t n);

		const T& operator[](size_t n) const;
	};

	template<typename T>
	class ObjectPool_Array {
		std::vector<T, pnTlsfInsertSTLtype<T>> valPool;
		BitMap64 is_used;

		uint32_t pointer;
		uint32_t bufsize;

	public:
		//	�R���X�g���N�^
		ObjectPool_Array(size_t size = 16);
		//	�f�X�g���N�^
		~ObjectPool_Array();

		//	�I�u�W�F�N�g���v�[������|�b�v����
		PoolArray<T> pop(size_t size);
		//	�I�u�W�F�N�g���v�[���ɕԋp����
		void push(PoolArray<T>* p);
		//	�v�[���̃T�C�Y��ύX����
		void resize(size_t size);

	private:
		//	�K�����u���b�N���v�[������T��
		T* search_pool_block(size_t size, size_t* indices);
		//	�r�b�g�}�b�v���g�p���Č�������
		inline uint32_t search_bit_area(uint8_t size) const;
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
	};
}