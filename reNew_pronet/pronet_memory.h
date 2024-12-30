#pragma once
#include <iostream>
#include <memory>

#include "ObjectPoolArray.h"
#include "ObjectPoolList.h"
#include "ObjectPool.h"

namespace pronet {
	template<typename T>
	class poolArray_unique_ptr
	{
		struct _Deleter {
			ObjectPool_Array<T>* _pool;
			
			_Deleter(pronet::ObjectPool_Array<T>* pool) noexcept :_pool(pool) {}

			void operator()(PoolArray<T>* ptr) {
				_pool->back(ptr);
				std::cout << "custom_unique_ptr_return object" << std::endl;
			}
		};

		std::unique_ptr<PoolArray<T>, _Deleter> sp;

	public:

		poolArray_unique_ptr(size_t n = 0, ObjectPool_Array<T>*pool = nullptr)
			: sp(pool ? (new PoolArray<T>(pool->get(n))) : (nullptr), _Deleter(pool)) {}

		//	スマートポインタを返す
		std::unique_ptr<PoolArray<T>> operator()() const { return sp; }
		//	内部のオブジェクトを返す
		T& operator[](size_t n) const {
			if (sp)
				return sp->operator[](n);
			else
				throw std::runtime_error("poolArray_unique_ptr is null. you must call .reset");
		}
		//	メモリを開放する
		void reset() { sp.reset(); }

		void realloc(size_t n, ObjectPool_Array<T>* pool) {
			sp = std::unique_ptr<PoolArray<T>, _Deleter>(new PoolArray<T>(pool->get(n)), _Deleter(pool));
		}

		explicit operator bool() {
			if (sp)
				return true;
			else
				return false;
		}

		poolArray_unique_ptr<PoolArray<T>>& operator=(const poolArray_unique_ptr<PoolArray<T>>& o) {
			if (this != &o) {
				this->sp = std::move(o.sp);
			}
			return *this;
		}

		PoolArray<T>* operator->() const { return sp.operator->(); }
	};
	
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
			if (this != &o) {
				this->sp = o.sp;
			}
			return *this;
		}

		T& operator[](size_t n) {
			return sp->operator[](n);
		}

		void reset() {
			sp.reset();
		}
	};

	template<class T, std::size_t N>
	class poolObject_shared_ptr {
		std::shared_ptr<Pool_Object<T>> sp;

		//	デリーター
		struct Deleter
		{
			//	オブジェクトプールのポインタ
			ObjectPool<T, N>* _pool;

			//	コンストラクタ
			Deleter(ObjectPool<T, N>* pool) : _pool(pool) {
			}

			//	オーバーロードされたオブジェクト返却の処理
			void operator()(Pool_Object<T>* ptr) {
				Pool_Object<T> buf = std::move(*ptr);
				_pool->push(ptr);
				std::cout << "custom shared ptr return object!" << std::endl;
			}
		};

	public:
		poolObject_shared_ptr(ObjectPool<T, N>* pool = nullptr)
		{
			if (pool != nullptr) {
				Pool_Object<T> buf = pool->pop();
				sp = std::shared_ptr<Pool_Object<T>>(new Pool_Object<T>(buf), Deleter(pool));
			}
		}

		std::shared_ptr<Pool_Object<T>> operator()() {
			return sp;
		}

		T* operator->() const {
			return sp->operator->();
		}

		poolObject_shared_ptr<Pool_Object<T>, N>& operator=(const poolArray_shared_ptr<Pool_Object<T>>& o) {
			if (this != &o) {
				this->sp = o.sp;
			}
			return *this;
		}

		T& operator[](size_t n) {
			return sp->data[n];
		}

		void reset() {
			sp.reset();
		}
	};
}