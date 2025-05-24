#pragma once
#include "pnFreeList.h"

/*
* 現状出てきた問題点
* ・戻り値をもっと厳格に（検索に特化）
* ・フリーリストをもっと厳格に（テンプレートである必要性を感じない）
* ・関数の役割がちぐはぐ
* ・リサイズ処理について
* 
* 関数規定
* ・コンストラクタ
* ・デストラクタ
* ・locate(size_t size)　指定要件のメモリブロックを返す
* ・
* 解決策
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
		tlsf_vector<fl_store_type_weak> data_;	//	TLSFのデータを保存する関数
		pronet::ObjectPool<pnFreeList, 6> flPool_;	//	フリーリストのプール

		fl_store_type_weak first_list;
		fl_store_type_shared final_list;

		uint64_t fli_;				//	FLI
		BitMap64 sli_;				//	SLI

		size_t bufSiz_;				//	プールのサイズ
		uint8_t bitdiv_;			//	SLIのインデックスのビット数
		size_t minsiz_;				//	SLIのインデックスに分類できる最小値
		uint64_t bitmsk_;			//	SLIのインデックスを割り出すマスク
		uint8_t fliLv_;				//	FLIのレベルの数
	public:
		tlsf_set(uint8_t _div_size, size_t _pool_size);
		virtual ~tlsf_set();

		//	オブジェクトを取得する
		//	size : オブジェクトのサイズ
		//	obj : オブジェクトの情報を保存するクラス
		bool get(size_t _size, obj_type& _obj);

		//	オブジェクトを返す
		//	obj : オブジェクトの情報を保存するクラス
		void back(obj_type& _obj);

		//	プールのサイズを変更する
		//	プールの新たなサイズ
		void resize(size_t _size);

		//	プールのサイズを圧縮する
		size_t compress();

		//	TLSFのインデックスを計算する
		//	_fli : ファーストレベルインデックス
		//	_sli : セカンドレベルインデックス
		//	_size : 計算するメモリブロックのサイズ
		void calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	レジスト可能なサイズかを確かめる
		[[nodiscard]] bool rigistable(size_t _size) const { return (_size >= minsiz_); }
		//	ビットマップを描画
		void print_bmp() const;
		void printFlMap() const;

	private:
		virtual [[nodiscard]] size_t Alignment(size_t _n) const;

	private:
		//	メモリブロックの割り当てを行う
		//	obj : メモリブロックオブジェクトのポインタ
		void rigist(fl_store_type_shared& _obj);

		//	指定のメモリブロックを削除する
		//	obj : メモリブロックオブジェクト
		void unrigist(fl_store_type_shared& _obj);
		//	指定のメモリブロックを削除する
		//	fli、sli、インデックスがわかっているのであればこちらを使う（計算を省略）
		//	obj : メモリブロックオブジェクト
		//	fli : ファーストレベルインデックス
		//	sli : セカンドレベルインデックス
		//	index : 所属するインデックス
		void unrigist(fl_store_type_shared& _obj, uint8_t _fli, uint8_t _sli, size_t _index);

		//	要求の満たしたメモリブロックを返す
		//	obj : メモリブロックオブジェクトのポインタ
		//	size : メモリブロックのサイズ
		fl_store_type_shared locate(size_t _size, uint8_t* const _fli = nullptr, uint8_t* const _sli = nullptr, size_t* const _index = nullptr);
		//	要求されたサイズを満たすのメモリブロックインデックスを探して返す
		//	_size : 要求サイズ
		bool search(uint8_t& _fli, uint8_t& _sli, size_t _size) const;

		//	ビットマップ関連のサイズをプールサイズに応じて調整する
		//	size : プールのサイズ
		void resize_bmp(size_t _size);

		//	メモリブロック不足が発生しないように1つ上のインデックスになるようにアラインメントをかける
		void align_idx(uint8_t& _fli, uint8_t& _sli) const;
		//	ビットマップにアンレジストの変更を反映させる
		void del_bmp(uint8_t _fli, uint8_t _sli, size_t _index);
		//	メモリブロックをアンレジスト時にリンクを解除する
		void delink(fl_store_type_shared _obj, size_t _index);
		//	SLIを計算する
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;

		//	サイズにアラインメントをかける
		[[nodiscard]] size_t sizeAlignment(size_t _n) const;
		//	FLIとSLIをもとに1次元配列のインデックスに変換
		[[nodiscard]] size_t calcIndex(uint8_t _fli, uint8_t _sli) const { return (_fli << bitdiv_) + _sli; }
		//	メモリのステータスをゲットする
		[[nodiscard]] bool getStat(uint8_t _fli, uint8_t _sli) const;
	};

	template<class _FL>
	class tlsf_set_noalign
	{
		std::vector<_FL*> data_;	//	TLSFのデータを保存する関数
		uint64_t fli_;				//	FLI
		BitMap64 sli_;				//	SLI

		uint8_t bitdiv_;			//	SLIのインデックスのビット数
		size_t minsiz_;				//	SLIのインデックスに分類できる最小値
		uint64_t bitmsk_;			//	SLIのインデックスを割り出すマスク
		uint8_t fliLv_;				//	FLIのレベルの数
	public:
		tlsf_set_noalign(uint8_t _div_size, uint8_t _lsb);
		~tlsf_set_noalign();
		//	メモリブロックの割り当てを行う
		//	_obj : メモリブロックオブジェクトのポインタ（attach(_FL* const _ptr)関数の定義が必要）
		//	_size : メモリブロックのサイズ（detach(_FL*& _ptr)関数の定義が必要）
		void rigist(_FL* const _obj, size_t _size);
		//	指定のメモリブロックを削除する
		//	_obj : メモリブロックオブジェクト
		//	_size : メモリブロックのサイズ
		void unrigist(_FL* const _obj, size_t _size);
		//	要求の満たしたメモリブロックを返す
		//	_obj : メモリブロックオブジェクトのポインタ
		//	_size : メモリブロックのサイズ
		_FL* locate(size_t _size);
		//	要求されたサイズを満たすのメモリブロックインデックスを探して返す
		//	_size : 要求サイズ
		bool search(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	TLSFのインデックスを計算する
		//	_fli : ファーストレベルインデックス
		//	_sli : セカンドレベルインデックス
		//	_size : 計算するメモリブロックのサイズ
		void calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;
		//	レジスト可能なサイズかを確かめる
		[[nodiscard]] bool rigistable(size_t _size) const { return (_size >= minsiz_); }
		//	ビットマップを描画
		void print_bmp() const;
	private:
		//	メモリブロック不足が発生しないように1つ上のインデックスになるようにアラインメントをかける
		void align_idx(uint8_t& _fli, uint8_t& _sli) const;
		//	ビットマップにアンレジストの変更を反映させる
		void del_bmp(uint8_t _fli, uint8_t _sli, size_t _index);
		//	メモリブロックをアンレジスト時にリンクを解除する
		void delink(_FL* const _obj, size_t _index);
		//	SLIを計算する
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;
		//	FLIとSLIをもとに1次元配列のインデックスに変換
		[[nodiscard]] size_t calcIndex(uint8_t _fli, uint8_t _sli) const { return (_fli << bitdiv_) + _sli; }
		//	メモリのステータスをゲットする
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