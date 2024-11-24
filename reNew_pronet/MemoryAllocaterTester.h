#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <functional>

namespace pronet {

	template<typename T, typename S, class M>
	class MemoryAllocaterTester
	{
		std::function<T> allocateMethod;

		std::function<S> deallocateMethod;

		M* manager;

		time_t testTime;

	public:

		//	自作メモリアロケータのテスト用
		//	メモリリークが起こらないかを調べる
		//	allocatemethod : 自作メモリアロケーターのメモリ確保する関数ポインタ
		//	deallocatemethod : 自作メモリアロケーターのメモリ開放する関数ポインタ
		//	manager : メモリアロケータのクラスのポインタ
		//	testtime : テストする時間（1秒当たり1で指定する。ミリ秒未対応）
		MemoryAllocaterTester(T* allocatemethod, S* deallocatemethod, M* manager, time_t testtime)
			: allocateMethod(allocatemethod)
			, deallocateMethod(deallocatemethod)
			, manager()
			, testTime(testtime)
		{
			srand(static_cast<unsigned>(time(nullptr)));
		}

		~MemoryAllocaterTester() {

		}

		void run(uint32_t maxsize, uint32_t minsize) {
			time_t nowtime(time(nullptr));
			time_t lasttime(time(nullptr));

			const uint8_t maxAllocation(16);

			uint32_t size(0);
			uint32_t allocateCount(0);
			uint32_t deallocateCount(0);
			std::vector<char*> ptr;
			while (nowtime - lasttime <= testTime) {

				allocateCount = rand() % maxAllocation;
				deallocateCount = rand() % maxAllocation;
				
				for (int i = 0; i < allocateCount; i++) {
					size = minsize + rand() % (maxsize - minsize);
					ptr.emplace_back(allocateMethod(size));
					for (int j = 0; j < size; j++) {
						if (j % 32 == 0)
							ptr.back()[j] = "T";
						else if (j % 8 == 0)
							ptr.back()[j] = "S";
						else
							ptr.back()[j] = "c";
					}
					std::cout << &ptr.back()[0] << std::endl;
				}
				for (int i = 0; i < deallocateCount; i++) {
					size = minsize + rand() % (maxsize - minsize);
					if (!ptr.empty()) {
						ptr.emplace_back(deallocateMethod(ptr.front()));
						ptr.erase(ptr.begin());
					}
				}

				nowtime(time(nullptr));
			}

			std::cout << "MemoryAllocateTest is Finish!!" << std::endl;
		}
	};
}

