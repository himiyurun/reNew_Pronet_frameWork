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
		T* data;		//	プールのポインタ
		size_t index;	//	ビットマップのインデックス
		size_t size;	//	実際のサイズ

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
		//	コンストラクタ
		ObjectPool_Array(size_t size = 16);
		//	デストラクタ
		~ObjectPool_Array();

		//	オブジェクトをプールからポップする
		PoolArray<T> pop(size_t size);
		//	オブジェクトをプールに返却する
		void push(PoolArray<T>* p);
		//	プールのサイズを変更する
		void resize(size_t size);

	private:
		//	適したブロックをプールから探す
		T* search_pool_block(size_t size, size_t* indices);
		//	ビットマップを使用して検索する
		inline uint32_t search_bit_area(uint8_t size) const;
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
	};
}