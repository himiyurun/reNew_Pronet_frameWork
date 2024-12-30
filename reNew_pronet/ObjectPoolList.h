#pragma once
#include <iostream>
#include <cassert>

namespace pronet {
	template<typename T>
	struct PoolArray {
		T* data;		//	プールのポインタ
		size_t index;	//	ビットマップのインデックス
		size_t size;	//	実際のサイズ

		//	コンストラクタ
		PoolArray(T* data = nullptr, size_t index = 0, size_t size = 0) : data(data), index(index), size(size) {}

		//	コピーコンストラクタ
		PoolArray(const PoolArray& o) : data(o.data), index(o.index), size(o.size) {}

		// ムーブコンストラクタ
		PoolArray(PoolArray&& o) noexcept
			: data(o.data), index(o.index), size(o.size)
		{
			o.data = nullptr;
			o.index = 0;
			o.size = 0;
		}

		// コピー代入演算子
		PoolArray& operator=(const PoolArray<T>& o) {
			if (this != &o) {
				this->data = o.data;
				this->index = o.index;
				this->size = o.size;
			}
			return *this;
		}

		T& operator[](size_t n)
		{
			assert(data && "Error : PoolArray.operator[](size_t) : data is null");
			if (n < size)
				return data[n];
			else
				throw std::out_of_range("PoolArray.operator[](size_t) out of range");
		}

		const T& operator[](size_t n) const
		{
			assert(data && "Error : PoolArray.operator[](size_t) : data is null");
			if (n < size)
				return data[n];
			else
				throw std::out_of_range("PoolArray.operator[](size_t) out of range");
		}
	};
}