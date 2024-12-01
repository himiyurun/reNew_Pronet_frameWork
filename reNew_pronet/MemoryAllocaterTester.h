#pragma once
#include <iostream>
#include <vector>

#include "TLSFmemory.h"
#include "Timer.h"

namespace pronet {


	void runMemoryAllocater(uint32_t maxsize, uint32_t minsize, time_t testTime, pronet::TLSFmemory* tlsf, uint64_t* alc, uint64_t* dlc) {
		time_t nowtime(time(nullptr));
		time_t lasttime(time(nullptr));

		const uint8_t maxAllocation(32);
		uint64_t allocCount(0);
		uint64_t deallocCount(0);

		uint32_t size(0);
		uint32_t allocateCount(0);
		uint32_t deallocateCount(0);
		pronet::BoundaryTagBegin* begin(nullptr);
		pronet::BoundaryTagEnd* end(nullptr);
		std::vector<char*> ptr;
		while (nowtime - lasttime <= testTime) {

			allocateCount = rand() % maxAllocation;
			deallocateCount = rand() % maxAllocation;

			for (int i = 0; i < allocateCount; i++) {
				{
					Timer time;

					size = minsize + rand() % (maxsize - minsize);
					char* alocPtr = static_cast<char*>(tlsf->allocate(size));
#ifdef _DEBUG
					std::cout << "Allocate Size : " << size << std::endl;
#endif
					if (alocPtr) {
						ptr.emplace_back(alocPtr);
						char* buf = ptr.back();
#ifdef _DEBUG
						begin = reinterpret_cast<pronet::BoundaryTagBegin*>(buf - pronet::begSize);
						end = begin->endTag();
						std::cout << "true size : " << begin->bufSize() << ", " << end->size << std::endl;
#endif			
						for (int j = 0; j < size - 1; j++) {
							if (j % 32 == 0)
								buf[j] = 'T';
							else if (j % 8 == 0)
								buf[j] = 'S';
							else
								buf[j] = 'c';
						}
						buf[size - 1] = '\0';
						//std::cout << &ptr.back()[0] << std::endl;
						allocCount++;
					}
#ifdef _DEBUG
					else
					{
						std::cout << std::endl << "May be Memory Pool is FULL" << std::endl << std::endl;
					}
					tlsf->printFreelistStatus();
					std::cout << std::endl;
					tlsf->printMemoryLayout();
					std::cout << std::endl;
#endif // _DEBUG
				}
			}
			for (int i = 0; i < deallocateCount; i++) {
				{
					Timer time;
					size = minsize + rand() % (maxsize - minsize);
					if (!ptr.empty()) {
						tlsf->deallocate(ptr.front());
						ptr.erase(ptr.begin());
						deallocCount++;
					}
				}
#ifdef _DEBUG
				tlsf->printFreelistStatus();
				std::cout << std::endl;
				tlsf->printMemoryLayout();
				std::cout << std::endl;
#endif
			}

			nowtime = time(nullptr);
		}

		for (auto& a : ptr) {
			if (a)
				tlsf->deallocate(a);
		}

		*alc += allocCount;
		*dlc += deallocCount;
		std::cout << "Allocate count : " << allocCount << ", deallocate count : " << deallocCount << std::endl;
		std::cout << "MemoryAllocateTest is Finish!!" << std::endl;
	}

	void runNormalAllocater(uint32_t maxsize, uint32_t minsize, time_t testTime, uint64_t* alc, uint64_t* dlc) {
		time_t nowtime(time(nullptr));
		time_t lasttime(time(nullptr));

		const uint8_t maxAllocation(32);
		uint64_t allocCount(0);
		uint64_t deallocCount(0);

		uint32_t size(0);
		uint32_t allocateCount(0);
		uint32_t deallocateCount(0);
		std::vector<char*> ptr;
		while (nowtime - lasttime <= testTime) {

			nowtime = time(nullptr);

			allocateCount = rand() % maxAllocation;
			deallocateCount = rand() % maxAllocation;

			for (int i = 0; i < allocateCount; i++) {
				{
					Timer time;

					size = minsize + rand() % (maxsize - minsize);
					char* alocPtr = new char[size];
					if (alocPtr) {
						ptr.emplace_back(alocPtr);
						char* buf = ptr.back();
						for (int j = 0; j < size - 1; j++) {
							if (j % 32 == 0)
								buf[j] = 'T';
							else if (j % 8 == 0)
								buf[j] = 'S';
							else
								buf[j] = 'c';
						}
						buf[size - 1] = '\0';
						//std::cout << &ptr.back()[0] << std::endl;
						allocCount++;
					}
				}
			}
			for (int i = 0; i < deallocateCount; i++) {
				{
					Timer time;
					size = minsize + rand() % (maxsize - minsize);
					if (!ptr.empty()) {
						delete[] ptr[0];
						ptr.erase(ptr.begin());
						deallocCount++;
					}
				}
			}
		}

		for (char* a : ptr) {
			if (a)
				delete[] a;
		}

		*alc += allocCount;
		*dlc += deallocCount;
		std::cout << "Allocate count : " << allocCount << ", deallocate count : " << deallocCount << std::endl;
		std::cout << "MemoryAllocateTest is Finish!!" << std::endl;
	}

}

