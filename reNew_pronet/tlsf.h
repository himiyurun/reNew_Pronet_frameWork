#pragma once
#include "BitMap64.h"

namespace pronet {
	template<class _Ty>
	class tlsf_set
	{
		std::vector<_Ty*> data_;	//	TLSFのデータを保存する関数
		uint64_t fli_;				//	FLI
		BitMap64 sli_;				//	SLI

		uint8_t bitdiv_;			//	SLIのインデックスのビット数
		size_t bitsiz_;				//	SLIのインデックスに分類できる最小値
		uint8_t fliLv_;				//	FLIのレベルの数
		uint8_t minFli_;			//	FLIの最小値
	public:
		tlsf_set(uint8_t _div_size, uint8_t _lsb);
		~tlsf_set();
		//	メモリブロックの割り当てを行う
		//	_obj : メモリブロックオブジェクトのポインタ（attach(_Ty* const _ptr)関数の定義が必要）
		//	_size : メモリブロックのサイズ（detach(_Ty*& _ptr)関数の定義が必要）
		void rigist(_Ty* const _obj, size_t _size);
		//	メモリブロックの削除を行う
		//	_obj : メモリブロックオブジェクトのポインタ
		//	_size : メモリブロックのサイズ
		_Ty* unrigist(size_t _size);
		//	要求されたサイズを満たすのメモリブロックインデックスを探して返す
		//	_size : 要求サイズ
		bool search(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	TLSFのインデックスを計算する
		//	_fli : ファーストレベルインデックス
		//	_sli : セカンドレベルインデックス
		//	_size : 計算するメモリブロックのサイズ
		void calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
	private:
		//	メモリブロック不足が発生しないように1つ上のインデックスになるようにアラインメントをかける
		void align_idx(uint8_t& _fli, uint8_t& _sli) const;
		//	メモリブロックをアンレジスト時にリンクを解除する
		void delink(_Ty* const _obj, size_t _index);
		//	SLIを計算する
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;
		//	FLIとSLIをもとに1次元配列のインデックスに変換
		[[nodiscard]] size_t calcIndex(uint8_t _fli, uint8_t _sli) const { return (_fli << bitdiv_) + _sli; }
		//	メモリのステータスをゲットする
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