#pragma once
#include "BitMap64.h"

namespace pronet {
	template<class _Ty>
	class tlsf_set
	{
		std::vector<_Ty*> data_;	//	TLSF�̃f�[�^��ۑ�����֐�
		uint64_t fli_;				//	FLI
		BitMap64 sli_;				//	SLI

		uint8_t bitdiv_;			//	SLI�̃C���f�b�N�X�̃r�b�g��
		size_t bitsiz_;				//	SLI�̃C���f�b�N�X�ɕ��ނł���ŏ��l
		uint8_t fliLv_;				//	FLI�̃��x���̐�
		uint8_t minFli_;			//	FLI�̍ŏ��l
	public:
		tlsf_set(uint8_t _div_size, uint8_t _lsb);
		~tlsf_set();
		//	�������u���b�N�̊��蓖�Ă��s��
		//	_obj : �������u���b�N�I�u�W�F�N�g�̃|�C���^�iattach(_Ty* const _ptr)�֐��̒�`���K�v�j
		//	_size : �������u���b�N�̃T�C�Y�idetach(_Ty*& _ptr)�֐��̒�`���K�v�j
		void rigist(_Ty* const _obj, size_t _size);
		//	�������u���b�N�̍폜���s��
		//	_obj : �������u���b�N�I�u�W�F�N�g�̃|�C���^
		//	_size : �������u���b�N�̃T�C�Y
		_Ty* unrigist(size_t _size);
		//	�v�����ꂽ�T�C�Y�𖞂����̃������u���b�N�C���f�b�N�X��T���ĕԂ�
		//	_size : �v���T�C�Y
		bool search(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	TLSF�̃C���f�b�N�X���v�Z����
		//	_fli : �t�@�[�X�g���x���C���f�b�N�X
		//	_sli : �Z�J���h���x���C���f�b�N�X
		//	_size : �v�Z���郁�����u���b�N�̃T�C�Y
		void calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
	private:
		//	�������u���b�N�s�����������Ȃ��悤��1��̃C���f�b�N�X�ɂȂ�悤�ɃA���C�������g��������
		void align_idx(uint8_t& _fli, uint8_t& _sli) const;
		//	�������u���b�N���A�����W�X�g���Ƀ����N����������
		void delink(_Ty* const _obj, size_t _index);
		//	SLI���v�Z����
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;
		//	FLI��SLI�����Ƃ�1�����z��̃C���f�b�N�X�ɕϊ�
		[[nodiscard]] size_t calcIndex(uint8_t _fli, uint8_t _sli) const { return (_fli << bitdiv_) + _sli; }
		//	�������̃X�e�[�^�X���Q�b�g����
		[[nodiscard]] bool getStat(uint8_t _fli, uint8_t _sli) const;
	};

	template<class _Ty>
	inline tlsf_set<_Ty>::tlsf_set(uint8_t _div_size, uint8_t _lsb)
		: data_((_lsb + 1) << _div_size, nullptr)
		, fli_(0), sli_(((_div_size* (_lsb + 1)) >> BITCOUNT_OF_64) + 1)
		, bitdiv_(_div_size), bitsiz_(1ULL << _div_size)
		, fliLv_(_lsb), minFli_(1ULL << _div_size)
	{
	}

	template<class _Ty>
	inline tlsf_set<_Ty>::~tlsf_set()
	{
	}

	template<class _Ty>
	inline void tlsf_set<_Ty>::rigist(_Ty* const _obj, size_t _size)
	{
		uint8_t fli(0), sli(0);
		if (_size >= bitsiz_) {
			calcTlsfIndex(fli, sli, _size);
		}
		else {
			fli = minFli_;
		}

		if (!_bit_get_status(fli, UNSIGNED_INT_64, fli))
			_bit_write_one_area(&fli_, UNSIGNED_INT_64, fli, 1);
		
		size_t index(calcIndex(fli, sli));
		if (!sli[index]) {
			sli_.write_Bit_1(index, 1);
		}

		_obj.attach(data_[index]);
		data_[index] = _obj;
	}

	template<class _Ty>
	inline _Ty* tlsf_set<_Ty>::unrigist(size_t _size)
	{
		uint8_t fli(0), sli(0);
		if (!search(fli, sli, _size)) {
			return nullptr;
		}
		size_t index(calcIndex(fli, sli));
		_Ty* obj = data_[index];
		delink(obj, index);
		return obj;
	}

	template<class _Ty>
	inline bool tlsf_set<_Ty>::search(uint8_t& _fli, uint8_t& _sli, size_t _size) const
	{
		size_t fli(0), sli(0);
		calcTlsfIndex(fli, sli, _size);
		align_idx(fli, sli);

		if (_bit_get_status(fli_, UNSIGNED_INT_64, fli)) {
			if (sli_.find_one_from(calcIndex(fli, sli), &sli)) {
				_fli = fli;
				_sli = sli;
				return true;
			}
			else {
				throw std::logic_error("Not working TLSFrigist system!!");
			}
		}
		if (!_bit_find_one_from(fli_, UNSIGNED_INT_64, fli, &fli)) {
			std::cerr << "Log : Memory Block is Full!!" << std::endl;
			return false;
		}
		if (!sli_.find_one_from(calcIndex(fli, sli), &sli)) {
			throw std::logic_error("Not working TLSFrigist system!!");
		}
		_fli = fli;
		_sli = sli;
		return true;
	}

	template<class _Ty>
	inline void tlsf_set<_Ty>::calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const
	{
		unsigned long index;
		_BitScanReverse(&index, _size);
		_fli = (uint8_t)index;
		calcSLI(_sli, _fli, _size);
	}

	template<class _Ty>
	inline bool tlsf_set<_Ty>::getStat(uint8_t _fli, uint8_t _sli) const
	{
		size_t index((_fli << bitdiv_) + _sli);
		return sli_[index];
	}

	template<class _Ty>
	inline void tlsf_set<_Ty>::align_idx(uint8_t& _fli, uint8_t& _sli) const
	{
		_sli++;
		if (_sli >= bitsiz_) {
			_sli %= bitsiz_;
			_fli++;
		}
	}

	template<class _Ty>
	inline void tlsf_set<_Ty>::delink(_Ty* const _obj, size_t _index)
	{
		_obj.detach(data_[_index]);
	}

	template<class _Ty>
	inline void tlsf_set<_Ty>::calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const
	{
		uint8_t shift(0);
		shift = _fli - bitdiv_;
		_sli = (_size & ~(1 << _fli)) >> (shift);
	}
}