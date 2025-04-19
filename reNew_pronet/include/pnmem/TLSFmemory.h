#ifndef TLSF_MEMORY_H_
#define TLSF_MEMORY_H_
#include <vector>
#include <memory>

#include "BoundaryTag.h"

namespace pronet {

	class PNMEM_API TLSFmemory {
		uint8_t* pool;
		BoundaryTagBegin** freelist;
		size_t fl_size;

		uint16_t* parititionSLI;
		size_t SLI_size;
		uint64_t parititionFLI;

		size_t bufSize;
		uint8_t minSize;

		uint8_t divBit;
		uint8_t divSize;

	public:

		//	�R���X�g���N�^
		//	n : �v�[���̃������̃r�b�g��
		//	divsize : SLI�̕�����
		TLSFmemory(size_t n = 7, uint8_t divsize = 4);

		~TLSFmemory();

		//	�������̃A���P�[�V�������s��
		//	size : �A���P�[�V��������T�C�Y
		void* allocate(size_t size);

		BoundaryTagBegin* searchFreeBlock(uint8_t fli, uint8_t sli) const;

		//	���������J������
		//	p : ����������������u���b�N�̐擪�|�C���^
		void deallocate(void* p);

		bool margeNextfreeBlock(BoundaryTagBegin* begin, BoundaryTagBegin* next);
		bool margePrevfreeBlock(BoundaryTagBegin* prev, BoundaryTagBegin* &begin);

		//	�t���[���X�g�Ƀ^�O�����蓖�Ă�
		//	begin : ���蓖�Ă����^�O�̃|�C���^
		//	size : ���蓖�Ă����T�C�Y
		void rigist(BoundaryTagBegin* begin, size_t size);

		//	�t���[���X�g����^�O�����O��
		//	begin : ���蓖�Ă����^�O�̃|�C���^
		//	size : ���蓖�Ă����T�C�Y
		void unrigist(BoundaryTagBegin* begin, size_t size);

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
		void calcTlsfIndex(uint8_t* fli, uint8_t* sli, size_t size) const;

		void calcSLI(uint8_t* sli, uint8_t fli, size_t size) const;

		size_t sizeAlignment(size_t size) const { 
#ifdef _DEBUG
			std::cout << "original : " << size << ", alignment :" << ((size + 3) & ~(3)) << std::endl;
#endif
			return (size + 3) & ~(3); 
		}

		//	�r�b�g�̕`����s��
		//	num : �`����s�������ϐ�
		//	size : �r�b�g�̃T�C�Y
		void printBit(uint64_t num, uint8_t size) const;

		//	�t���[���X�g�̒��g�������Ă��邩��`�悷��
		void printFreelistStatus() const;

		//	�p���e�B�V�����r�b�g�̒��g��`��
		void printParititionBit() const;

		void printMemoryLayout() const;

		//	�������A���P�[�^�[�̃��C�A�E�g

		//	FLI��SLI�����Ƃ�1�����z��̃C���f�b�N�X�ɕϊ�
		[[nodiscard]] size_t calcIndex(uint8_t fli, uint8_t sli) const { return (fli << divBit) + sli; }

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
		[[nodiscard]] size_t size() const { return bufSize; }
	};
}
#endif