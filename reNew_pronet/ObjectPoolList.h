#pragma once
#include <iostream>
#include <cassert>

namespace pronet {
	template<typename T>
	struct PoolArray {
		T* data;		//	プールのポインタ
		size_t index;	//	ビットマップのインデックス
		size_t size;	//	実際のサイズ

		PoolArray(T* data = nullptr, size_t size = 0) 
			: data(data)
			, index(0)
			, size(size)
		{
		}

		PoolArray(PoolArray&& o) noexcept {
			o = *this;
			this->data = nullptr;
			this->index = 0;
			this->size = 0;
		}

		PoolArray& operator=(const PoolArray& o) {
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
		}

		const T& operator[](size_t n) const
		{
			assert(data && "Error : PoolArray.operator[](size_t) : data is null");
			if (n < size)
				return data[n];
		}
	};
}