#pragma once
#include "pnFreeList.h"

/*
* ����o�Ă������_
* �E�߂�l�������ƌ��i�Ɂi�����ɓ����j
* �E�t���[���X�g�������ƌ��i�Ɂi�e���v���[�g�ł���K�v���������Ȃ��j
* �E�֐��̖����������͂�
* �E���T�C�Y�����ɂ���
* 
* �֐��K��
* �E�R���X�g���N�^
* �E�f�X�g���N�^
* �Elocate(size_t size)�@�w��v���̃������u���b�N��Ԃ�
* �E
* ������
*/

namespace pronet {
	template<typename _Ty>
	using tlsf_vector = std::vector<_Ty, pnTlsfInsertSTLtype<_Ty>>;

	class tlsf_set
	{
		template<class _Ty>
		using smart_ptr_type_shared =	poolObject_shared_ptr<_Ty, _PN_FREE_LIST_LEVEL>;
		template<class _Ty>
		using smart_ptr_type_weak = std::weak_ptr<Pool_Object<_Ty>>;

	public:
		using free_list_type = pnFreeList;
		using fl_store_type_shared = smart_ptr_type_shared<free_list_type>;
		using fl_store_type_weak = smart_ptr_type_weak<free_list_type>;
		using obj_type = fl_store_type_shared;
	private:
		tlsf_vector<fl_store_type_weak> data_;	//	TLSF�̃f�[�^��ۑ�����֐�
		pronet::ObjectPool<pnFreeList, 6> flPool_;	//	�t���[���X�g�̃v�[��

		fl_store_type_weak first_list;
		fl_store_type_shared final_list;

		uint64_t fli_;				//	FLI
		BitMap64 sli_;				//	SLI

		size_t bufSiz_;				//	�v�[���̃T�C�Y
		uint8_t bitdiv_;			//	SLI�̃C���f�b�N�X�̃r�b�g��
		size_t minsiz_;				//	SLI�̃C���f�b�N�X�ɕ��ނł���ŏ��l
		uint64_t bitmsk_;			//	SLI�̃C���f�b�N�X������o���}�X�N
		uint8_t fliLv_;				//	FLI�̃��x���̐�
	public:
		tlsf_set(uint8_t _div_size, size_t _pool_size);
		virtual ~tlsf_set();

		//	�I�u�W�F�N�g���擾����
		//	size : �I�u�W�F�N�g�̃T�C�Y
		//	obj : �I�u�W�F�N�g�̏���ۑ�����N���X
		bool get(size_t _size, obj_type& _obj);

		//	�I�u�W�F�N�g��Ԃ�
		//	obj : �I�u�W�F�N�g�̏���ۑ�����N���X
		void back(obj_type& _obj);

		//	�v�[���̃T�C�Y��ύX����
		//	�v�[���̐V���ȃT�C�Y
		void resize(size_t _size);

		//	�v�[���̃T�C�Y�����k����
		size_t compress();

		//	TLSF�̃C���f�b�N�X���v�Z����
		//	_fli : �t�@�[�X�g���x���C���f�b�N�X
		//	_sli : �Z�J���h���x���C���f�b�N�X
		//	_size : �v�Z���郁�����u���b�N�̃T�C�Y
		void calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	���W�X�g�\�ȃT�C�Y�����m���߂�
		[[nodiscard]] bool rigistable(size_t _size) const { return (_size >= minsiz_); }
		//	�r�b�g�}�b�v��`��
		void print_bmp() const;
		void printFlMap() const;

	private:
		virtual [[nodiscard]] size_t Alignment(size_t _n) const;

	private:
		//	�������u���b�N�̊��蓖�Ă��s��
		//	obj : �������u���b�N�I�u�W�F�N�g�̃|�C���^
		void rigist(fl_store_type_shared& _obj);

		//	�w��̃������u���b�N���폜����
		//	obj : �������u���b�N�I�u�W�F�N�g
		void unrigist(fl_store_type_shared& _obj);
		//	�w��̃������u���b�N���폜����
		//	fli�Asli�A�C���f�b�N�X���킩���Ă���̂ł���΂�������g���i�v�Z���ȗ��j
		//	obj : �������u���b�N�I�u�W�F�N�g
		//	fli : �t�@�[�X�g���x���C���f�b�N�X
		//	sli : �Z�J���h���x���C���f�b�N�X
		//	index : ��������C���f�b�N�X
		void unrigist(fl_store_type_shared& _obj, uint8_t _fli, uint8_t _sli, size_t _index);

		//	�v���̖��������������u���b�N��Ԃ�
		//	obj : �������u���b�N�I�u�W�F�N�g�̃|�C���^
		//	size : �������u���b�N�̃T�C�Y
		fl_store_type_shared locate(size_t _size, uint8_t* const _fli = nullptr, uint8_t* const _sli = nullptr, size_t* const _index = nullptr);
		//	�v�����ꂽ�T�C�Y�𖞂����̃������u���b�N�C���f�b�N�X��T���ĕԂ�
		//	_size : �v���T�C�Y
		bool search(uint8_t& _fli, uint8_t& _sli, size_t _size) const;

		//	�r�b�g�}�b�v�֘A�̃T�C�Y���v�[���T�C�Y�ɉ����Ē�������
		//	size : �v�[���̃T�C�Y
		void resize_bmp(size_t _size);

		//	�������u���b�N�s�����������Ȃ��悤��1��̃C���f�b�N�X�ɂȂ�悤�ɃA���C�������g��������
		void align_idx(uint8_t& _fli, uint8_t& _sli) const;
		//	�r�b�g�}�b�v�ɃA�����W�X�g�̕ύX�𔽉f������
		void del_bmp(uint8_t _fli, uint8_t _sli, size_t _index);
		//	�������u���b�N���A�����W�X�g���Ƀ����N����������
		void delink(fl_store_type_shared _obj, size_t _index);
		//	SLI���v�Z����
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;

		//	�T�C�Y�ɃA���C�������g��������
		[[nodiscard]] size_t sizeAlignment(size_t _n) const;
		//	FLI��SLI�����Ƃ�1�����z��̃C���f�b�N�X�ɕϊ�
		[[nodiscard]] size_t calcIndex(uint8_t _fli, uint8_t _sli) const { return (_fli << bitdiv_) + _sli; }
		//	�������̃X�e�[�^�X���Q�b�g����
		[[nodiscard]] bool getStat(uint8_t _fli, uint8_t _sli) const;
	};

	template<class _FL>
	class tlsf_set_noalign
	{
		std::vector<_FL*> data_;	//	TLSF�̃f�[�^��ۑ�����֐�
		uint64_t fli_;				//	FLI
		BitMap64 sli_;				//	SLI

		uint8_t bitdiv_;			//	SLI�̃C���f�b�N�X�̃r�b�g��
		size_t minsiz_;				//	SLI�̃C���f�b�N�X�ɕ��ނł���ŏ��l
		uint64_t bitmsk_;			//	SLI�̃C���f�b�N�X������o���}�X�N
		uint8_t fliLv_;				//	FLI�̃��x���̐�
	public:
		tlsf_set_noalign(uint8_t _div_size, uint8_t _lsb);
		~tlsf_set_noalign();
		//	�������u���b�N�̊��蓖�Ă��s��
		//	_obj : �������u���b�N�I�u�W�F�N�g�̃|�C���^�iattach(_FL* const _ptr)�֐��̒�`���K�v�j
		//	_size : �������u���b�N�̃T�C�Y�idetach(_FL*& _ptr)�֐��̒�`���K�v�j
		void rigist(_FL* const _obj, size_t _size);
		//	�w��̃������u���b�N���폜����
		//	_obj : �������u���b�N�I�u�W�F�N�g
		//	_size : �������u���b�N�̃T�C�Y
		void unrigist(_FL* const _obj, size_t _size);
		//	�v���̖��������������u���b�N��Ԃ�
		//	_obj : �������u���b�N�I�u�W�F�N�g�̃|�C���^
		//	_size : �������u���b�N�̃T�C�Y
		_FL* locate(size_t _size);
		//	�v�����ꂽ�T�C�Y�𖞂����̃������u���b�N�C���f�b�N�X��T���ĕԂ�
		//	_size : �v���T�C�Y
		bool search(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	TLSF�̃C���f�b�N�X���v�Z����
		//	_fli : �t�@�[�X�g���x���C���f�b�N�X
		//	_sli : �Z�J���h���x���C���f�b�N�X
		//	_size : �v�Z���郁�����u���b�N�̃T�C�Y
		void calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	���W�X�g�\�ȃT�C�Y�����m���߂�
		[[nodiscard]] bool rigistable(size_t _size) const { return (_size >= minsiz_); }
		//	�r�b�g�}�b�v��`��
		void print_bmp() const;
	private:
		//	�������u���b�N�s�����������Ȃ��悤��1��̃C���f�b�N�X�ɂȂ�悤�ɃA���C�������g��������
		void align_idx(uint8_t& _fli, uint8_t& _sli) const;
		//	�r�b�g�}�b�v�ɃA�����W�X�g�̕ύX�𔽉f������
		void del_bmp(uint8_t _fli, uint8_t _sli, size_t _index);
		//	�������u���b�N���A�����W�X�g���Ƀ����N����������
		void delink(_FL* const _obj, size_t _index);
		//	SLI���v�Z����
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;
		//	FLI��SLI�����Ƃ�1�����z��̃C���f�b�N�X�ɕϊ�
		[[nodiscard]] size_t calcIndex(uint8_t _fli, uint8_t _sli) const { return (_fli << bitdiv_) + _sli; }
		//	�������̃X�e�[�^�X���Q�b�g����
		[[nodiscard]] bool getStat(uint8_t _fli, uint8_t _sli) const;
	};
	template<class _FL>
	inline tlsf_set_noalign<_FL>::tlsf_set_noalign(uint8_t _div_size, uint8_t _lsb)
		: data_((uint64_t)(_lsb + 1) << _div_size, nullptr)
		, fli_(0), sli_((((1 << _div_size)* (_lsb + 1)) >> BITCOUNT_OF_64) + 1)
		, bitdiv_(_div_size), minsiz_(1ULL << _div_size), bitmsk_((1ULL << _div_size) - 1)
		, fliLv_(_lsb)
	{
	}

	template<class _FL>
	inline tlsf_set_noalign<_FL>::~tlsf_set_noalign()
	{
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::rigist(_FL* const _obj, size_t _size)
	{
		uint8_t fli(0), sli(0);
		if (_size >= minsiz_) {
			calcTlsfIndex(fli, sli, _size);
			std::cout << "size : " << _size << ", fli : " << (unsigned)fli << ", sli : " << (unsigned)sli << std::endl;
		}
		else {
			size_t idx(0);
			_bit_find_one_from_reverse(_size, sizeof(size_t) * 8, 0, &idx);
			fli = idx;
		}

		if (!_bit_get_status(fli, UNSIGNED_INT_64, fli))
			_bit_write_one_area(&fli_, UNSIGNED_INT_64, fli, 1);

		size_t index(calcIndex(fli, sli));
		if (!sli_[index]) {
			sli_.write_Bit_1(index, 1);
		}

		_obj->attach(data_[index]);
		data_[index] = _obj;
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::unrigist(_FL* const _obj, size_t _size)
	{
		uint8_t fli(0), sli(0);
		if (_size >= minsiz_) {
			calcTlsfIndex(fli, sli, _size);
			std::cout << "size : " << _size << ", fli : " << (unsigned)fli << ", sli : " << (unsigned)sli << std::endl;
		}
		else {
			size_t idx(0);
			_bit_find_one_from_reverse(_size, sizeof(size_t) * 8, 0, &idx);
			fli = idx;
		}
		size_t index(calcIndex(fli, sli));

		if (!data_[index]) {
			throw std::runtime_error("This Object have already delinked!!");
		}
		delink(_obj, index);
		del_bmp(fli, sli, index);
	}

	template<class _FL>
	inline _FL* tlsf_set_noalign<_FL>::locate(size_t _size)
	{
		uint8_t fli(0), sli(0);
		if (!search(fli, sli, _size)) {
			return nullptr;
		}

		size_t index(calcIndex(fli, sli));
		_FL* obj = data_[index];
		delink(obj, index);
		del_bmp(fli, sli, index);
		return obj;
	}

	template<class _FL>
	inline bool tlsf_set_noalign<_FL>::search(uint8_t& _fli, uint8_t& _sli, size_t _size) const
	{
		uint8_t f(0), s(0);
		calcTlsfIndex(f, s, _size);
		if (_size >= minsiz_)
			align_idx(f, s);
		else
			s = 0;
		std::cout << "fli : " << (unsigned)f << ", sli : " << (unsigned)s << std::endl;
		size_t fli(f), sli(s);
		if (_bit_get_status(fli_, UNSIGNED_INT_64, fli)) {
			if (sli_.find_one_from(calcIndex(fli, sli), &sli)) {
				_fli = fli;
				_sli = (sli & bitmsk_);
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
		_sli = (sli & bitmsk_);
		return true;
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const
	{
		if (_size >= minsiz_) {
			unsigned long index;
			_BitScanReverse(&index, _size);
			_fli = (uint8_t)index;
			calcSLI(_sli, _fli, _size);
		}
		else {
			size_t idx(0);
			_bit_find_one_from(_size, sizeof(size_t) * 8, 0, &idx);
			_fli = idx;
			_sli = 0;
		}
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::print_bmp() const
	{
		std::cout << "FLI : ";
		_bit_print(fli_);
		for (uint8_t i = 0; i < fliLv_; i++) {
			for (size_t j = 0; j < minsiz_; j++) {
				if (sli_[i * minsiz_ + j])
					std::cout << "1";
				else
					std::cout << "0";
			}
			std::cout << '\n';
		}
	}

	template<class _FL>
	inline bool tlsf_set_noalign<_FL>::getStat(uint8_t _fli, uint8_t _sli) const
	{
		size_t index((_fli << bitdiv_) + _sli);
		return sli_[index];
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::align_idx(uint8_t& _fli, uint8_t& _sli) const
	{
		_sli++;
		if (_sli >= minsiz_) {
			_sli %= minsiz_;
			_fli++;
		}
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::del_bmp(uint8_t _fli, uint8_t _sli, size_t _index)
	{
		if (!data_[_index]) {
			sli_.write_Bit_0(((uint64_t)_fli << bitdiv_) + _sli, 1);
			if (!sli_.extract_area((uint64_t)_fli << bitdiv_, minsiz_)) {
				_bit_write_zero_area(&fli_, UNSIGNED_INT_64, _fli, 1);
			}
		}
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::delink(_FL* const _obj, size_t _index)
	{
		_obj->detach(data_[_index]);
	}

	template<class _FL>
	inline void tlsf_set_noalign<_FL>::calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const
	{
		uint8_t shift(0);
		shift = _fli - bitdiv_;
		_sli = (_size & ~(1 << _fli)) >> (shift);
	}	

}