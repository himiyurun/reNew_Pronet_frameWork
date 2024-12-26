#pragma once
#include <iostream>
#include <memory>

#include "ObjectPoolArray.h"
#include "ObjectPoolList.h"

namespace pronet {
	template<typename T>
	class poolArray_shared_ptr
	{
		std::shared_ptr<PoolArray<T>> sp;

		//	デリーター
		struct Deleter 
		{
			//	オブジェクトプールのポインタ
			ObjectPool_Array<T>* _pool;

			//	コンストラクタ
			Deleter(pronet::ObjectPool_Array<T>* pool) : _pool(pool) {
			}

			//	オーバーロードされたオブジェクト返却の処理
			void operator()(PoolArray<T>* ptr) {
				_pool->back(ptr);
				std::cout << "custom shared ptr return object!" << std::endl;
			}
		};

	public:
		poolArray_shared_ptr(std::size_t size = 0, ObjectPool_Array<T>* pool = nullptr) 
		{
			if (size > 0 && pool != nullptr)
				sp = std::shared_ptr<PoolArray<T>>(new PoolArray<T>(pool->get(size)), Deleter(pool));
		}

		PoolArray<T> operator()() const {
			return sp;
		}

		poolArray_shared_ptr<PoolArray<T>>& operator=(const poolArray_shared_ptr<PoolArray<T>>& o) {
			if (this != o) {
				this->sp = o.sp;
			}
			return *this;
		}

		T& operator[](size_t n) {
			return sp->data[n];
		}
	};
}