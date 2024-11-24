#pragma once
#include <vector>
#include <new>

#include "BoundaryTag.h"

namespace pronet {

	class TLSFmemory {
		std::vector<uint8_t> pool;
		std::vector<BoundaryTagBegin*> freelist;

		std::vector<uint16_t> parititionSLI;
		uint64_t parititionFLI;

		uint32_t bufSize;
		uint8_t minSize;

		uint8_t divBit;
		uint8_t divSize;

	public:

		//	�R���X�g���N�^
		//	n : �v�[���̃������̃r�b�g��
		//	divsize : SLI�̕�����
		TLSFmemory(uint32_t n, uint8_t divsize);

		~TLSFmemory();

		//	�������̃A���P�[�V�������s��
		//	size : �A���P�[�V��������T�C�Y
		void* allocate(uint32_t size);

		BoundaryTagBegin* searchFreeBlock(uint8_t fli, uint8_t sli) const;

		//	���������J������
		//	p : ����������������u���b�N�̐擪�|�C���^
		void deallocate(void* p);

		bool margeNextfreeBlock(BoundaryTagBegin* begin, BoundaryTagBegin* next);
		bool margePrevfreeBlock(BoundaryTagBegin* prev, BoundaryTagBegin* &begin);

		//	�t���[���X�g�Ƀ^�O�����蓖�Ă�
		//	begin : ���蓖�Ă����^�O�̃|�C���^
		//	size : ���蓖�Ă����T�C�Y
		void rigist(BoundaryTagBegin* begin, uint32_t size);

		//	�t���[���X�g����^�O�����O��
		//	begin : ���蓖�Ă����^�O�̃|�C���^
		//	size : ���蓖�Ă����T�C�Y
		void unrigist(BoundaryTagBegin* begin, uint32_t size);

		void writeParititionBit(uint8_t fli, uint8_t sli);

		void clearParititionBit(uint8_t fli, uint8_t sli);

		//	�p���e�B�V�����r�b�g�ɏ�������
		//	fli : FLI�̒l
		//	sli : SLI�̒l
		void writeBit(uint8_t fli, uint8_t sli);

		//	�p���e�B�V�����r�b�g���N���A����
		//	fli : FLI�̒l
		//	sli : SLI�̒l
		void clearBit(uint8_t fli, uint8_t sli);

		//	�T�C�Y����FLI��SLI���Z�o
		//	fli : FLI�̒l���i�[����|�C���^
		//	sli : SLI�̒l���i�[����|�C���^
		//	size : �Z�o�������T�C�Y
		void calcTlsfIndex(uint8_t* fli, uint8_t* sli, uint32_t size) const;

		void calcSLI(uint8_t* sli, uint8_t fli, uint32_t size) const;

		uint32_t sizeAlignment(uint32_t size) const { return (size + 0x03) & ~(0x03); }

		//	�r�b�g�̕`����s��
		//	num : �`����s�������ϐ�
		//	size : �r�b�g�̃T�C�Y
		void printBit(uint64_t num, uint8_t size) const;

		//	�t���[���X�g�̒��g�������Ă��邩��`�悷��
		void printFreelistStatus() {
			for (int i = 0; i < freelist.size(); i++) {
				if (i % divSize == 0) {
					std::cout << std::endl;
					std::cout << "num" << i / divSize << " : ";
				}
				
				if (freelist[i]) {
					std::cout << "1";
				}
				else {
					std::cout << "0";
				}
			}
			std::cout << std::endl << std::endl;
		}

		//	�p���e�B�V�����r�b�g�̒��g��`��
		void printParititionBit();

		//	FLI��SLI�����Ƃ�1�����z��̃C���f�b�N�X�ɕϊ�
		[[nodiscard]] uint32_t calcIndex(uint8_t fli, uint8_t sli) const { return (fli << divBit) + sli; }

		//	�ŉ��ʃr�b�g��Ԃ��@0�̂Ƃ���false��Ԃ�
		//	num : �ŉ��ʃr�b�g�����߂����ϐ�
		[[nodiscard]] bool getLSB(uint64_t num, uint8_t* lsb) const;

		//	FLI�̔ԍ�����g���Ă��邩�𔻒�
		//	fli : FLI�̔ԍ�
		[[nodiscard]] bool fli_used(uint8_t fli) const;
		//	SLI�̔ԍ�����g���Ă��邩�𔻒�
		//	sli : SLI�̔ԍ�
		[[nodiscard]] bool sli_used(uint8_t fli, uint8_t sli) const;
		//	�������v�[���̃T�C�Y
		[[nodiscard]] uint32_t size() const { return bufSize; }
	};

}