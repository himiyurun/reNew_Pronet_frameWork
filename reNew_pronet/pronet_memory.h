#pragma once
#include <iostream>
#include <utility>
#include <memory>

#include "ObjectPoolArray.h"
#include "ObjectPoolList.h"
#include "ObjectPool.h"
#include "pnTlsf.h"

namespace pronet {
	/*
	void _print_memory(const void* _ptr, size_t size) {
		uint8_t* ptr = reinterpret_cast<uint8_t*>(ptr);
		for (size_t i = 0; i < size; i++) {
			for (size_t i = 0; i < UNSIGNED_INT_8; i++) {
				if (*ptr & (static_cast<uint64_t>(1) << i))
					std::cout << "1";
				else
					std::cout << "0";
			}
			ptr++;
		}
		std::cout << '\n';
	}
	*/

	template<typename T>
	class pnTlsf_unique_ptr : public pnTlsf {
		struct _Deleter : public pnTlsf {
			_Deleter() {}

			void operator()(T* ptr){
				delete_type<T>(ptr);
				std::cout << "pn_Tlsf unique_ptr return object" << std::endl;
			}
		};
		std::unique_ptr<T, _Deleter> sp;
		std::size_t buf_size;

	public:
		pnTlsf_unique_ptr(size_t n = 0)
			: sp(n ? (new_type<T>(n)) : (nullptr), _Deleter())
			, buf_size(n)
		{}

		pnTlsf_unique_ptr(pnTlsf_unique_ptr&& o) noexcept : sp(std::move(o.sp)) {}

		//	スマートポインタを返す
		std::unique_ptr<T, _Deleter>& operator()() const { return sp; }
		//	内部のオブジェクトを返す
		T& operator[](size_t n) const {
			if (sp)
				return sp.get()[n];
			else
				throw std::runtime_error("pnTlsf_unique_ptr is null. you must call .realloc");
		}
		//	メモリを開放する
		void reset() { sp.reset(); }

		void realloc(size_t n) {
			sp = std::unique_ptr<T, _Deleter>(new_type<T>(n), _Deleter());
			buf_size = n;
		}

		explicit operator bool() {
			if (sp)
				return true;
			else
				return false;
		}

		pnTlsf_unique_ptr<T>& operator=(pnTlsf_unique_ptr<T>& o) noexcept {
			if (this != &o) {
				this->sp = std::move(o.sp);
				this->buf_size = o.buf_size;
				o.buf_size = 0;
			}
			return *this;
		}

		T* operator->() const { return sp.operator->(); }

		[[nodiscard]] T* begin() const { return sp.get(); }
		[[nodiscard]] T* end() const { return sp ? sp.get() + buf_size : nullptr; }

		[[nodiscard]] size_t size() const { return buf_size; }
	};

	template<class _Ty, std::size_t _N>
	class poolObject_unique_ptr 
	{
		struct _Deleter {
			ObjectPool<_Ty, _N>* pool_;

			_Deleter(pronet::ObjectPool<_Ty, _N>* _pool) noexcept :pool_(_pool) {}

			void operator()(Pool_Object<_Ty>* _ptr) {
				pool_->push(_ptr);
				std::cout << "object_pool_array unique_ptr return object" << std::endl;
			}
		};

		std::unique_ptr<Pool_Object<_Ty>, _Deleter> sp_;

	public:

		poolObject_unique_ptr(ObjectPool<_Ty, _N>* _pool = nullptr)
			: sp_(_pool ? (new Pool_Object<_Ty>(_pool->pop())) : (nullptr), _Deleter(_pool)) {
		}

		poolObject_unique_ptr(poolObject_unique_ptr&& o) noexcept : sp_(std::move(o.sp_)) {}

		//	スマートポインタを返す
		std::unique_ptr<Pool_Object<_Ty>, _Deleter>& operator()() { return sp_; }
		//	内部のオブジェクトを返す
		explicit operator _Ty&() {
			if (sp_)
				return sp_->operator->();
			else
				throw std::runtime_error("poolObject_unique_ptr is null. you must call .realloc");
		}
		//	メモリを開放する
		void reset() { sp_.reset(); }

		void realloc(ObjectPool<_Ty, _N>* _pool) {
			sp_ = std::unique_ptr<Pool_Object<_Ty>, _Deleter>(new Pool_Object<_Ty>(_pool->pop()), _Deleter(_pool));
			if (!sp_)
				throw std::runtime_error("ObjectPool_Array allocation failed!");
		}

		explicit operator bool() {
			if (sp_)
				return true;
			else
				return false;
		}

		poolObject_unique_ptr<_Ty, _N>& operator=(const poolObject_unique_ptr<_Ty, _N>& o) noexcept {
			if (this != &o) {
				this->sp_ = std::move(o.sp_);
			}
			return *this;
		}

		Pool_Object<_Ty>* operator->() const { return sp_.operator->(); }
	};

	template<typename T>
	class poolArray_unique_ptr
	{
		struct _Deleter {
			ObjectPool_Array<T>* _pool;
			
			_Deleter(pronet::ObjectPool_Array<T>* pool) noexcept :_pool(pool) {}

			void operator()(PoolArray<T>* ptr) {
				_pool->back(ptr);
				std::cout << "object_pool_array unique_ptr return object" << std::endl;
			}
		};

		std::unique_ptr<PoolArray<T>, _Deleter> sp;

	public:

		poolArray_unique_ptr(size_t n = 0, ObjectPool_Array<T>*pool = nullptr)
			: sp(pool ? (new PoolArray<T>(pool->get(n))) : (nullptr), _Deleter(pool)) {}

		poolArray_unique_ptr(poolArray_unique_ptr&& o) noexcept : sp(std::move(o.sp)) {}

		~poolArray_unique_ptr() {
			sp.reset();
		}

		//	スマートポインタを返す
		std::unique_ptr<PoolArray<T>, _Deleter>& operator()() { return sp; }
		//	内部のオブジェクトを返す
		T& operator[](size_t n) const {
			if (sp)
				return sp->operator[](n);
			else
				throw std::runtime_error("poolArray_unique_ptr is null. you must call .realloc");
		}
		//	メモリを開放する
		void reset() { sp.reset(); }

		void realloc(size_t n, ObjectPool_Array<T>* pool) {
			sp = std::unique_ptr<PoolArray<T>, _Deleter>(new PoolArray<T>(pool->get(n)), _Deleter(pool));
			if (!sp)
				throw std::runtime_error("ObjectPool_Array allocation failed!");
		}

		explicit operator bool() {
			if (sp)
				return true;
			else
				return false;
		}

		poolArray_unique_ptr<T>& operator=(const poolArray_unique_ptr<T>& o) noexcept {
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
				std::cout << "object_pool_array shared ptr return object" << std::endl;
			}
		};

	public:
		poolArray_shared_ptr(std::size_t size = 0, ObjectPool_Array<T>* pool = nullptr) 
		{
			if (size > 0 && pool != nullptr)
				sp = std::shared_ptr<PoolArray<T>>(new PoolArray<T>(pool->get(size)), Deleter(pool));
		}
		
		~poolArray_shared_ptr() {
			sp.reset();
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
				buf->reset();
				_pool->push(ptr);
				std::cout << "pool_object shared ptr return object" << std::endl;
			}
		};

	public:
		poolObject_shared_ptr(ObjectPool<T, N>* pool = nullptr)
		{
			if (pool != nullptr) {
				sp = std::shared_ptr<Pool_Object<T>>(new Pool_Object<T>(pool->pop()), Deleter(pool));
			}
		}

		template<class T, std::size_t N>
		poolObject_shared_ptr(poolObject_shared_ptr<T, N>& o) noexcept
		{
			sp = o.sp;
		}

		~poolObject_shared_ptr() {
			sp.reset();
		}

		void realloc(ObjectPool<T, N>* _pool) {
			sp.reset();
			sp = std::shared_ptr<Pool_Object<T>>(new Pool_Object<T>(_pool->pop()), Deleter(_pool));
			if (!sp)
				throw std::runtime_error("ObjectPoo allocation failed!");
		}

		std::shared_ptr<Pool_Object<T>> operator()() const {
			return sp;
		}

		explicit operator bool() const {
			return sp.operator bool();
		}

		T* get() const { return sp->operator->(); }

		T* operator->() const {
			//_print_memory(this, sizeof(*this));
			return sp->operator->();
		}

		poolObject_shared_ptr<T, N>& operator=(const poolObject_shared_ptr& o) {
			if (this != &o) {
				this->sp = o.sp;
			}
			return *this;
		}

		T& operator[](size_t n) {
			return sp->operator->()[n];
		}

		bool get_index() const {
			Pool_Object<T>* const ptr = sp.get();
			std::cout << "index : " << ptr->index << std::endl;
			//std::cout << "count : " << this->sp.use_count() << std::endl;
			if (sp) {
				std::cout << "exsist" << std::endl;
				return true;
			}
			else {
				std::cout << "null" << std::endl;
				return false;
			}
		}

		void reset() {
			sp.reset();
		}
	};
}