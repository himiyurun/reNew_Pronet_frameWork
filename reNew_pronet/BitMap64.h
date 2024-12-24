#pragma once
#include <iostream>
#include <vector>
#include <cassert>

namespace pronet {
	class BitMap64
	{
		std::vector<uint64_t> bit;

	public:
		BitMap64(size_t size)
			: bit(size, 0)
		{
		}

		~BitMap64() {}

		bool get_bit_map(size_t offset) const {
			uint32_t current(offset / 64);
			uint32_t index(offset % 64);

			assert(current < bit.size() && "Error : BitMap64.find_zero_from : out of range");
			if (bit[current] & (static_cast<uint64_t>(1) << index))
				return true;
			else
				return false;
		}

		bool find_zero_first(size_t* i) const {
			unsigned long index(64);
			uint32_t counter(0);
			int current(0);
			for (uint64_t a : bit) {
				if (_BitScanForward64(&index, ~(a))) {
					*i = static_cast<size_t>(index) + current * 64;
					return true;
				}
				current++;
			}
			return false;
		}

		bool find_zero_from(size_t start, size_t* idx) const {
			uint32_t current(start / 64);
			uint32_t start_index(start % 64);
			unsigned long index(0);

			assert(start < bit.size() * 64 && "Error : BitMap64.find_one_from : out of range");

			if (_BitScanForward64(&index, ~(bit[current] >> start_index))) {
				*idx = static_cast<size_t>(index) + 64 * current + start_index;
				return true;
			}

			current++;
			for (size_t i = current; i < bit.size(); i++) {
				if (_BitScanForward64(&index, ~(bit[i]))) {
					*idx = static_cast<size_t>(index) + 64 * i;
					return true;
				}
			}
			return false;
		}

		bool find_one_from(size_t start, size_t* idx) const {
			uint32_t current(start / 64);
			uint32_t start_index(start % 64);
			unsigned long index(0);

			assert(start < bit.size() * 64 && "Error : BitMap64.find_one_from : out of range");

			if (_BitScanForward64(&index, bit[current] >> start_index)) {
				*idx = static_cast<size_t>(index) + 64 * current + start_index;
				return true;
			}

			current++;
			for (size_t i = current; i < bit.size(); i++) {
				if (_BitScanForward64(&index, bit[i])) {
					*idx = static_cast<size_t>(index) + 64 * i;
					return true;
				}
			}
			return false;
		}

		void write_Bit_1(size_t start, size_t size) {
			uint32_t current(start / 64);
			uint32_t index(start % 64);
			uint32_t bufsize(0);
			uint32_t last_current(current + 1);

			assert(start < bit.size() * 64 && "Error : BitMap64.write_Bit : out of range");

			if (size < 64 - index) {
				for (size_t i = 0; i < size; i++) {
					bit[current] |= (uint64_t)1 << (index + i);
				}
				return;
			}
			for (size_t i = 0; i < 64 - index; i++) {
				bit[current] |= (uint64_t)1 << (index + i);
			}
			bufsize = size - (64 - index);

			assert(current < bit.size() && "Error : BitMap64.write_Bit : out of range");

			for (size_t i = 0; i < bufsize; i++) {
				if (i % 64 == 0) current++;
				bit[current] |= (uint64_t)1 << (i % 64);
			}
		}

		void write_Bit_0(size_t start, size_t size) {
			uint32_t current(start / 64);
			uint32_t index(start % 64);
			uint32_t bufsize(0);

			assert(start < bit.size() * 64 && "Error : BitMap64.write_Bit : out of range");

			if (size < 64 - index) {
				for (size_t i = 0; i < size; i++) {
					bit[current] &= ~((uint64_t)1 << (index + i));
				}
				return;
			}
			for (size_t i = 0; i < 64 - index; i++) {
				bit[current] &= ~((uint64_t)1 << (index + i));
			}
			bufsize = size - (64 - index);

			assert(current < bit.size() && "Error : BitMap64.write_Bit : out of range");

			for (size_t i = 0; i < bufsize; i++) {
				if (i % 64 == 0) current++;
				bit[current] &= ~((uint64_t)1 << (i % 64));
			}
		}

		void resize(size_t size) {
			bit.resize(size, 0);
		}

		void printBit() const {
			for (uint64_t num : bit) {
				for (int i = 0; i < 64; i++) {
					if (num & (static_cast<uint64_t>(1) << i))
						std::cout << "1";
					else
						std::cout << "0";
				}
				std::cout << std::endl;
			}
		}

		[[nodiscard]] size_t size() const { return bit.size(); }
	};
}