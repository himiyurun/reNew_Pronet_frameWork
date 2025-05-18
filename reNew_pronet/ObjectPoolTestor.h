#pragma once
#include <ctime>
#include "ObjectPool.h"

template<class _Ty, std::size_t _Lv>
class ObjectPoolTestor {
private:
	pronet::ObjectPool<_Ty, _Lv> pool_;
	std::vector<pronet::Pool_Object<_Ty>> cache_;
	size_t max_count_;
	time_t start_time_;
	time_t test_time_;
public:
	//	init_size : 初期化するプールサイズ
	//	max_test_count : テストするカウントの最大値
	//	test_time : テストする時間
	ObjectPoolTestor(size_t _init_size = 32, size_t _max_test_count = 64, time_t _test_time = 60);

	~ObjectPoolTestor() = default;

	void run();

private:

	void setup();
	void allocate(size_t _size);
	void deallocate(size_t _size);
};

template<class _Ty, std::size_t _Lv>
inline ObjectPoolTestor<_Ty, _Lv>::ObjectPoolTestor(size_t _init_size, size_t _max_test_count, time_t _test_time)
	: pool_(_init_size)
	, max_count_(_max_test_count)
	, start_time_(0)
	, test_time_(_test_time)
{
}

template<class _Ty, std::size_t _Lv>
inline void ObjectPoolTestor<_Ty, _Lv>::run()
{
	time_t now = time(nullptr);
	while (now < start_time_ + test_time_) {
		int proc_stat = rand() % 2;
		size_t proc_count = rand() % max_count_;
		if (!proc_stat) {
			deallocate(proc_count);
		}
		else {
			allocate(proc_count);
		}
		for (const auto& a : cache_) {
			std::cout << a.index << std::endl;
		}
		now = time(nullptr);
	}
}

template<class _Ty, std::size_t _Lv>
inline void ObjectPoolTestor<_Ty, _Lv>::setup()
{
	start_time_ = time(nullptr);
	srand(static_cast<unsigned>(time(nullptr)));
}

template<class _Ty, std::size_t _Lv>
inline void ObjectPoolTestor<_Ty, _Lv>::allocate(size_t _size)
{
	std::cout << "allocate : " << _size << std::endl;
	cache_.reserve(cache_.size() + _size);
	for (size_t i = 0; i < _size; i++) {
		std::cout << "count : " << i << std::endl;
		cache_.emplace_back(pool_.pop());
		std::cout << "id : " << cache_.back().index << std::endl;
		pool_.get_bit_to_pool_status();
	}
	std::cout << "finish" << std::endl;
}

template<class _Ty, std::size_t _Lv>
inline void ObjectPoolTestor<_Ty, _Lv>::deallocate(size_t _size)
{
	size_t deallocCt(_size);
	if (cache_.size() < _size) {
		deallocCt = cache_.size();
		std::cout << "small ";
	}
	std::cout << "deallocate : " << _size << ", cache_size : " << cache_.size() << std::endl;

	for (size_t i = 0; i < deallocCt; i++) {
		if (cache_.empty()) {
			std::cerr << "cache is empty" << std::endl;
			return;
		}
		std::cout << "count : " << i << std::endl;
		std::cout << "id : " << cache_.back().index << std::endl;
		if (cache_.back().index == 0) {
			std::cout << "size : " << cache_.size() << std::endl;
		}
		pool_.push(&cache_.back());
		cache_.pop_back();
		pool_.get_bit_to_pool_status();
	}
	std::cout << "cache_size : " << cache_.size() << ", finish" << std::endl;
}
